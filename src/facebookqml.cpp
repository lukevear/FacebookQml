#include "facebookqml.h"

static FacebookQml *_instance = 0;

FacebookQml::FacebookQml(QQuickItem *parent) : QQuickItem(parent)
{
    _platformImpl = QSharedPointer<FacebookQmlPlatformImpl>(new FacebookQmlPlatformImpl);
}

void FacebookQml::registerQmlContext()
{
    qmlRegisterSingletonType<FacebookQml>("FacebookQml", 1, 0, "Facebook", FacebookQml::instance);
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

void FacebookQml::logEvent(QString event)
{
    _platformImpl->logEvent(event);
}

void FacebookQml::logEvent(QString event, double valueToSum)
{
    _platformImpl->logEvent(event, valueToSum);
}

void FacebookQml::logEvent(QString event, QMap<QString, QVariant> parameters)
{
    _platformImpl->logEvent(event, parameters);
}

void FacebookQml::logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters)
{
    _platformImpl->logEvent(event, valueToSum, parameters);
}

void FacebookQml::login(QStringList permissions)
{
    _platformImpl->login(permissions);
}

void FacebookQml::logout()
{
    _platformImpl->logout();
}

QString FacebookQml::accessToken()
{
    return _platformImpl->accessToken();
}