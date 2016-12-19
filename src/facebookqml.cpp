#include "facebookqml.h"
#include "login/facebooklogin.h"
#include "share/facebookshare.h"

#if defined(Q_OS_ANDROID)
#include <QtAndroid>
#endif

static FacebookQml *_instance = 0;

FacebookQml::FacebookQml(QQuickItem *parent) : QQuickItem(parent)
{
#if defined(Q_OS_ANDROID)
    // Register our activity handler
    JNINativeMethod methods[] {
        {"FacebookQmlOnActivityResult", "(IILandroid/content/Intent;)Z", reinterpret_cast<void *>(JNIOnActivityResult)}
    };

    QAndroidJniEnvironment env;
    jclass objectClass = env->GetObjectClass(QtAndroid::androidActivity().object<jobject>());
    env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

    if (QAndroidJniObject::isClassAvailable("com/facebook/FacebookSdk")) {
        // Setup the Facebook SDK
        QAndroidJniObject::callStaticMethod<void>(
            "com/facebook/FacebookSdk",
            "sdkInitialize",
            "(Landroid/content/Context;)V",
            QtAndroid::androidActivity().object<jobject>()
        );

        // Register the Facebook callback manager
        _FBCallbackManager = QAndroidJniObject::callStaticObjectMethod(
            "com/facebook/CallbackManager$Factory",
            "create",
            "()Lcom/facebook/CallbackManager;"
        );
    }
#endif
}

void FacebookQml::registerQmlContext()
{
    qmlRegisterSingletonType<FacebookLogin>("Facebook.Login", 1, 0, "FacebookLogin", FacebookLogin::instance);
    qmlRegisterSingletonType<FacebookShare>("Facebook.Share", 1, 0, "FacebookShare", FacebookShare::instance);
}

FacebookQml *FacebookQml::instance()
{
    if (_instance == 0) {
        _instance = new FacebookQml();
    }

    return _instance;
}

QObject *FacebookQml::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return FacebookQml::instance();
}

#if defined(Q_OS_ANDROID)
QAndroidJniObject FacebookQml::getFBCallbackManager()
{
	return _FBCallbackManager;
}

bool FacebookQml::JNIOnActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    return FacebookQml::instance()->getFBCallbackManager().callMethod<jboolean>(
        "onActivityResult",
        "(IILandroid/content/Intent;)Z",
        requestCode,
        resultCode,
        data
    );
}
#endif
