#include "../facebookqml.h"

void FacebookQml::sdkSetup()
{
	//
}

void FacebookQml::sdkCleanUp()
{
	//
}

void FacebookQml::login(QStringList permissions)
{
	Q_UNUSED(permissions);
	emit loginError("Facebook Login is not supported on this platform.");
}

void FacebookQml::logout()
{
	//
}

QString FacebookQml::accessToken()
{
	return "";
}