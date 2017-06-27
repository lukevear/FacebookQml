#include "../facebookqmlplatformimpl.h"
#include "../facebookqml.h"

#include <QDebug>

FacebookQmlPlatformImpl::FacebookQmlPlatformImpl()
{
	
}

FacebookQmlPlatformImpl::~FacebookQmlPlatformImpl()
{
	
}

void FacebookQmlPlatformImpl::logEvent(QString event)
{
    qDebug() << "[FacebookQml] Event logging is not supported on this platform.";
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum)
{
    qDebug() << "[FacebookQml] Event logging is not supported on this platform.";
}

void FacebookQmlPlatformImpl::logEvent(QString event, QMap<QString, QVariant> parameters)
{
    qDebug() << "[FacebookQml] Event logging is not supported on this platform.";
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters)
{
    qDebug() << "[FacebookQml] Event logging is not supported on this platform.";
}

void FacebookQmlPlatformImpl::login(QStringList permissions)
{
	qDebug() << "[FacebookQml] Facebook Login is not supported on this platform. Will fire the loginFailed signal.";
	emit FacebookQml::instance()->loginFailed("Facebook Login is not supported on this platform.");
}

void FacebookQmlPlatformImpl::logout()
{
	qDebug() << "[FacebookQml] Facebook Login is not supported on this platform.";
}

QString FacebookQmlPlatformImpl::accessToken()
{
	qDebug() << "[FacebookQml] Facebook Login is not supported on this platform. Will return an empty string for accessToken().";

	return "";
}
