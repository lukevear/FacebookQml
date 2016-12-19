#include "facebooklogin.h"

static FacebookLogin *_instance = 0;

#if defined(Q_OS_IOS)
#include "ios/facebookloginios.h"
#elif defined(Q_OS_ANDROID)
#include "android/facebookloginandroid.h"
#endif

FacebookLogin::FacebookLogin(QQuickItem *parent) : QQuickItem(parent)
{

}

FacebookLogin *FacebookLogin::instance()
{
    if (_instance == 0) {
        _instance = new FacebookLogin();
    }

    return _instance;
}

QObject *FacebookLogin::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return FacebookLogin::instance();
}

void FacebookLogin::login(QStringList permissions)
{
#if defined(Q_OS_IOS)
    FacebookLoginiOS::instance()->login(permissions);
#elif defined(Q_OS_ANDROID)
    FacebookLoginAndroid::instance()->login(permissions);
#else
    Q_UNUSED(permissions);
    emit FacebookLogin::instance()->error("Facebook Login is not supported on this platform.");
#endif
}

void FacebookLogin::logout()
{
#if defined(Q_OS_IOS)
    FacebookLoginiOS::instance()->logout();
#elif defined(Q_OS_ANDROID)
    FacebookLoginAndroid::instance()->logout();
#else
    emit FacebookLogin::instance()->error("Facebook Login is not supported on this platform.");
#endif
}

QString FacebookLogin::accessToken()
{
#if defined(Q_OS_IOS)
    return FacebookLoginiOS::instance()->accessToken();
#elif defined(Q_OS_ANDROID)
    return FacebookLoginAndroid::instance()->accessToken();
#else
    return "";
#endif
}
