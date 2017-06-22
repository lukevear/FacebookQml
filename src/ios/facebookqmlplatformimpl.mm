#include "../facebookqmlplatformimpl.h"
#include "facebookqmliosutils.h"
#include <FBSDKCoreKit/FBSDKAppEvents.h>
#include <FBSDKLoginKit/FBSDKLoginKit.h>
#include "../facebookqml.h"

FacebookQmlPlatformImpl::FacebookQmlPlatformImpl()
{
	_loginManager = [[FBSDKLoginManager alloc] init];
}

FacebookQmlPlatformImpl::~FacebookQmlPlatformImpl()
{
	[static_cast<FBSDKLoginManager *>(_loginManager) release];
}

void FacebookQmlPlatformImpl::logEvent(QString event)
{
    [FBSDKAppEvents logEvent:event.toNSString()];
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum)
{
    [FBSDKAppEvents logEvent:event.toNSString() valueToSum:valueToSum];
}

void FacebookQmlPlatformImpl::logEvent(QString event, QMap<QString, QVariant> parameters)
{
    [FBSDKAppEvents logEvent:event.toNSString() parameters:FacebookQmlIosUtils::mapToDictionary(parameters)];
}

void FacebookQmlPlatformImpl::logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters)
{
    [FBSDKAppEvents logEvent:event.toNSString() valueToSum:valueToSum parameters:FacebookQmlIosUtils::mapToDictionary(parameters)];
}

void FacebookQmlPlatformImpl::login(QStringList permissions)
{
    NSMutableArray* permissionsArray = FacebookQmlIosUtils::stringListToArray(permissions);

	// Check if we are already authed with the same permissions
	if ([FBSDKAccessToken currentAccessToken]) {
        if ([[NSSet setWithArray:permissionsArray] isEqualToSet:[[FBSDKAccessToken currentAccessToken] permissions]]) {
			emit FacebookQml::instance()->loginSuccessful();
            return;
		}
	}

    [static_cast<FBSDKLoginManager *>(_loginManager) logInWithReadPermissions:permissionsArray
        fromViewController:[[UIApplication sharedApplication].keyWindow rootViewController]
        handler:^(FBSDKLoginManagerLoginResult *result, NSError *FBError) {
            if (FBError) {
                emit FacebookQml::instance()->loginFailed(QString::fromNSString([FBError localizedDescription]));
            } else if (result.isCancelled) {
                emit FacebookQml::instance()->loginCancelled();
            } else {
                emit FacebookQml::instance()->loginSuccessful();
            }
        }
    ];
}

void FacebookQmlPlatformImpl::logout()
{
	[static_cast<FBSDKLoginManager *>(_loginManager) logOut];
}

QString FacebookQmlPlatformImpl::accessToken()
{
	if ([FBSDKAccessToken currentAccessToken] != nil) {
		return QString::fromNSString([[FBSDKAccessToken currentAccessToken] tokenString]);
	}

	return "";
}
