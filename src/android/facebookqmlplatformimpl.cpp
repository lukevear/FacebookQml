#include "../facebookqmlplatformimpl.h"
#include "facebookqmlandroidutils.h"
#include "../facebookqml.h"

#include <QtAndroid>
#include <QAndroidJniObject>

FacebookQmlPlatformImpl::FacebookQmlPlatformImpl()
{
    if (FacebookQmlAndroidUtils::getInstance().isValid()) {
        _callbackManager = QAndroidJniObject::callStaticObjectMethod(
            "com/facebook/CallbackManager$Factory",
            "create",
            "()Lcom/facebook/CallbackManager;"
        );

        JNINativeMethod methods[] {
            {"notifyLoginSuccess", "()V", reinterpret_cast<void *>(JNINotifyLoginSuccess)},
            {"notifyLoginFailed", "(Ljava/lang/String;)V", reinterpret_cast<void *>(JNINotifyLoginFailed)},
            {"notifyLoginCancelled", "()V", reinterpret_cast<void *>(JNINotifyLoginCancelled)},
            {"onActivityResult", "(IILandroid/content/Intent;)Z", reinterpret_cast<void *>(JNIOnActivityResult)}
        };

        QAndroidJniEnvironment env;
        jclass objectClass = env->GetObjectClass(FacebookQmlAndroidUtils::getInstance().object<jobject>());
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
            FacebookQmlAndroidUtils::getInstance().object<jobject>()
        );
    }
}

FacebookQmlPlatformImpl::~FacebookQmlPlatformImpl()
{
    
}

void FacebookQmlPlatformImpl::logEvent(QString event)
{
    QtAndroid::runOnAndroidThread([event]() {
        QAndroidJniObject _logger = FacebookQmlAndroidUtils::getLogger();
        if (_logger.isValid()) {
            _logger.callMethod<void>(
                "logEvent",
                "(Ljava/lang/String;)V",
                QAndroidJniObject::fromString(event).object<jstring>()
            );
        }
    });
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum)
{
    QtAndroid::runOnAndroidThread([event, valueToSum]() {
        QAndroidJniObject _logger = FacebookQmlAndroidUtils::getLogger();
        if (_logger.isValid()) {
            _logger.callMethod<void>(
                "logEvent",
                "(Ljava/lang/String;D)V",
                QAndroidJniObject::fromString(event).object<jstring>(),
                static_cast<jdouble>(valueToSum)
            );
        }
    });
}

void FacebookQmlPlatformImpl::logEvent(QString event, QMap<QString, QVariant> parameters)
{
    QAndroidJniObject bundle = FacebookQmlAndroidUtils::mapToBundle(parameters);
    if (! bundle.isValid()) {
        return;
    }

    QtAndroid::runOnAndroidThread([event, bundle]() {
        QAndroidJniObject _logger = FacebookQmlAndroidUtils::getLogger();
        if (_logger.isValid()) {
            _logger.callMethod<void>(
                "logEvent",
                "(Ljava/lang/String;Landroid/os/Bundle;)V",
                QAndroidJniObject::fromString(event).object<jstring>(),
                bundle.object<jobject>()
            );
        }
    });
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters)
{
    QAndroidJniObject bundle = FacebookQmlAndroidUtils::mapToBundle(parameters);
    if (! bundle.isValid()) {
        return;
    }

    QtAndroid::runOnAndroidThread([event, valueToSum, bundle]() {
        QAndroidJniObject _logger = FacebookQmlAndroidUtils::getLogger();
        if (_logger.isValid()) {
            _logger.callMethod<void>(
                "logEvent",
                "(Ljava/lang/String;DLandroid/os/Bundle;)V",
                QAndroidJniObject::fromString(event).object<jstring>(),
                static_cast<jdouble>(valueToSum),
                bundle.object<jobject>()
            );
        }
    });
}

void FacebookQmlPlatformImpl::login(QStringList permissions)
{
    if (! QAndroidJniObject::isClassAvailable("com/facebook/login/LoginManager")) {
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
        FacebookQmlAndroidUtils::stringListToArrayList(permissions).object<jobject>()
    );
}

void FacebookQmlPlatformImpl::logout()
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
}

QString FacebookQmlPlatformImpl::accessToken()
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

void FacebookQmlPlatformImpl::JNINotifyLoginSuccess(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookQml::instance()->loginSuccessful();
}

void FacebookQmlPlatformImpl::JNINotifyLoginFailed(JNIEnv *env, jobject thiz, jobject error)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookQml::instance()->loginFailed(QAndroidJniObject(error).toString());
}

void FacebookQmlPlatformImpl::JNINotifyLoginCancelled(JNIEnv *env, jobject thiz)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    emit FacebookQml::instance()->loginCancelled();
}

bool FacebookQmlPlatformImpl::JNIOnActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data)
{
    Q_UNUSED(env);
    Q_UNUSED(thiz);

    return FacebookQml::instance()->_platformImpl->_callbackManager.callMethod<jboolean>(
        "onActivityResult",
        "(IILandroid/content/Intent;)Z",
        requestCode,
        resultCode,
        data
    );
}
