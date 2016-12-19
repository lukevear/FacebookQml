#include "facebookloginandroid.h"

#include "../facebooklogin.h"
#include "facebookqml.h"

#include <QtAndroid>
#include <QAndroidJniObject>

static FacebookLoginAndroid *_instance = 0;

FacebookLoginAndroid::FacebookLoginAndroid()
{
    if (QAndroidJniObject::isClassAvailable("com/lukevear/facebookqml/login/FacebookLoginCallbackManager")) {
        // Create a callback manager
        QAndroidJniObject loginCallbackManager = QAndroidJniObject("com/lukevear/facebookqml/login/FacebookLoginCallbackManager");

        // Register our hooks in the login callback manager
        JNINativeMethod methods[] {
            {"notifyLoginSuccess", "()V", reinterpret_cast<void *>(JNINotifyLoginSuccess)},
            {"notifyLoginError", "(Ljava/lang/String;)V", reinterpret_cast<void *>(JNINotifyLoginError)},
            {"notifyLoginCancelled", "()V", reinterpret_cast<void *>(JNINotifyLoginCancelled)}
        };

        QAndroidJniEnvironment env;
        jclass objectClass = env->GetObjectClass(loginCallbackManager.object<jobject>());
        env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
        env->DeleteLocalRef(objectClass);

        // Register the callback manager with the Facebook SDK
        QAndroidJniObject::callStaticObjectMethod(
            "com/facebook/login/LoginManager",
            "getInstance",
            "()Lcom/facebook/login/LoginManager;"
        ).callMethod<void>(
            "registerCallback",
            "(Lcom/facebook/CallbackManager;Lcom/facebook/FacebookCallback;)V",
            FacebookQml::instance()->getFBCallbackManager().object<jobject>(),
            loginCallbackManager.object<jobject>()
        );
    }
}

FacebookLoginAndroid *FacebookLoginAndroid::instance()
{
    if (_instance == 0) {
        _instance = new FacebookLoginAndroid();
    }

    return _instance;
}

void FacebookLoginAndroid::login(QStringList permissions)
{
    if (! QAndroidJniObject::isClassAvailable("com/facebook/login/LoginManager")) {
        return;
    }

    QAndroidJniObject _permissions = QAndroidJniObject("java/util/ArrayList");

    for (int i = 0; i < permissions.size(); ++i) {
        _permissions.callMethod<jboolean>(
            "add",
            "(Ljava/lang/Object;)Z",
            QAndroidJniObject::fromString(permissions.at(i)).object<jstring>()
        );
    }

    QAndroidJniObject::callStaticObjectMethod(
        "com/facebook/login/LoginManager",
        "getInstance",
        "()Lcom/facebook/login/LoginManager;"
    ).callMethod<void>(
        "logInWithReadPermissions",
        "(Landroid/app/Activity;Ljava/util/Collection;)V",
        QtAndroid::androidActivity().object<jobject>(),
        _permissions.object<jobject>()
    );
}

void FacebookLoginAndroid::logout()
{
    if (! QAndroidJniObject::isClassAvailable("com/facebook/login/LoginManager")) {
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

    emit FacebookLogin::instance()->accessTokenChanged();
}

QString FacebookLoginAndroid::accessToken()
{
    if (! QAndroidJniObject::isClassAvailable("com/facebook/AccessToken")) {
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

void FacebookLoginAndroid::JNINotifyLoginSuccess(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookLogin::instance()->authenticated();
    emit FacebookLogin::instance()->accessTokenChanged();
}

void FacebookLoginAndroid::JNINotifyLoginError(JNIEnv *env, jobject thiz, jobject error)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookLogin::instance()->error(QAndroidJniObject(error).toString());
}

void FacebookLoginAndroid::JNINotifyLoginCancelled(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookLogin::instance()->cancelled();
}
