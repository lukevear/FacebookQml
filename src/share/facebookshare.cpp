#include "facebookshare.h"

static FacebookShare *_instance = 0;

#if defined(Q_OS_IOS)
#include "ios/facebookshareios.h"
#endif

FacebookShare::FacebookShare(QQuickItem *parent) : QQuickItem(parent)
{

}

FacebookShare *FacebookShare::instance()
{
    if (_instance == 0) {
        _instance = new FacebookShare();
    }

    return _instance;
}

QObject *FacebookShare::instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return FacebookShare::instance();
}

void FacebookShare::imageDialog(QString filePath)
{
#if defined(Q_OS_IOS)
	FacebookShareiOS::imageDialog(filePath);
#endif
}