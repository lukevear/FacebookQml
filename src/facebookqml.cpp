#include "facebookqml.h"

static FacebookQml *_instance = 0;

FacebookQml::FacebookQml(QQuickItem *parent) : QQuickItem(parent)
{
    _sdkReady = false;
    sdkSetup();
}

FacebookQml::~FacebookQml()
{
    sdkCleanUp();
}

void FacebookQml::registerQmlContext()
{
    qmlRegisterSingletonType<FacebookQml>("FacebookQml", 2, 0, "Facebook", FacebookQml::instance);
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
