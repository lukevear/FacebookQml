#include "../facebookqml.h"

#include <QtAndroid>
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

#include "facebookqmlutils.h"

void FacebookQml::sdkSetup()
{
    if (
        ! QAndroidJniObject::isClassAvailable("com/facebook/AccessToken") ||
        ! QAndroidJniObject::isClassAvailable("com/facebook/login/LoginManager") ||
        ! QAndroidJniObject::isClassAvailable("com/lukevear/facebookqml/FacebookQml")
    ) {
        return;
    }

	// Create a callback manager
    _callbackManager = QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/CallbackManager$Factory",
        "create",
        "()Lcom/facebook/CallbackManager;"
    );

    // Retrieve our helper class
    _facebookQml = QAndroidJniObject::callStaticObjectMethod(
    	"com/lukevear/facebookqml/FacebookQml",
    	"getInstance",
    	"(Landroid/content/Context;)Lcom/lukevear/facebookqml/FacebookQml;",
    	QtAndroid::androidActivity().object<jobject>()
	);

    // Ensure that everything is valid
    if (! _callbackManager.isValid() || ! _facebookQml.isValid()) {
        return;
    }

    // Register our native callback methods
    JNINativeMethod methods[] {
        {"notifySuccess", "()V", reinterpret_cast<void *>(onSuccess)},
        {"notifyCancelled", "()V", reinterpret_cast<void *>(onCancel)},
        {"notifyError", "(Ljava/lang/String;)V", reinterpret_cast<void *>(onError)},
        {"notifyAccessTokenChanged", "()V", reinterpret_cast<void *>(onAccessTokenChanged)},
        {"onActivityResult", "(IILandroid/content/Intent;)Z", reinterpret_cast<void *>(onActivityResult)}
    };

    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(_facebookQml.object<jobject>());
    env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

    QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/login/LoginManager",
        "getInstance",
        "()Lcom/facebook/login/LoginManager;"
    ).callMethod<void>(
        "registerCallback",
        "(Lcom/facebook/CallbackManager;Lcom/facebook/FacebookCallback;)V",
        _callbackManager.object<jobject>(),
        _facebookQml.object<jobject>()
    );

    _sdkReady = true;
}

void FacebookQml::sdkCleanUp()
{
    if (! _sdkReady) {
        return;
    }

    _facebookQml.callMethod<void>(
        "stopTracking",
        "()V"
    );
}

void FacebookQml::login(QStringList permissions)
{
    if (! _sdkReady) {
        return;
    }

    QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/login/LoginManager",
        "getInstance",
        "()Lcom/facebook/login/LoginManager;"
    ).callMethod<void>(
        "logInWithReadPermissions",
        "(Landroid/app/Activity;Ljava/util/Collection;)V",
        QtAndroid::androidActivity().object<jobject>(),
        FacebookQmlUtils::stringListToArrayList(permissions).object<jobject>()
    );
}

void FacebookQml::logout()
{
    if (! _sdkReady) {
        return;
    }

    QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/login/LoginManager",
        "getInstance",
        "()Lcom/facebook/login/LoginManager;"
    ).callMethod<void>(
        "logOut",
        "()V"
    );
}

QString FacebookQml::accessToken()
{
    if (! _sdkReady) {
        return "";
    }

    QAndroidJniObject _accessToken = QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/AccessToken",
        "getCurrentAccessToken",
        "()Lcom/facebook/AccessToken;"
    );

    if (! _accessToken.isValid()) {
        return "";
    }

    return _accessToken.callObjectMethod(
        "getToken",
        "()Ljava/lang/String;"
    ).toString();
}

void FacebookQml::onSuccess(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit instance()->loginSuccessful();
}

void FacebookQml::onCancel(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

	emit instance()->loginCancelled();
}

void FacebookQml::onError(JNIEnv *env, jobject thiz, jobject error)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit instance()->loginError(QAndroidJniObject(error).toString());
}

void FacebookQml::onAccessTokenChanged(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit instance()->accessTokenChanged();
}

void FacebookQml::onActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    instance()->handleActivityResult(requestCode, resultCode, data);
}

void FacebookQml::handleActivityResult(int requestCode, int resultCode, jobject data)
{
    // While sdkSetup should be called before this method, we can't guarantee it
    if (! _sdkReady) {
        return;
    }

    _callbackManager.callMethod<jboolean>(
        "onActivityResult",
        "(IILandroid/content/Intent;)Z",
        requestCode,
        resultCode,
        data
    );
}
