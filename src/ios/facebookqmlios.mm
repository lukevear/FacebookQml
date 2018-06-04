#include "../facebookqml.h"

#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

#include "facebookqmlutils.h"

void FacebookQml::sdkSetup()
{
	_loginManager = [[FBSDKLoginManager alloc] init];

    _FBSDKObserver = [[NSNotificationCenter defaultCenter] addObserverForName:FBSDKAccessTokenDidChangeNotification
        object:nil
        queue:[NSOperationQueue mainQueue]
        usingBlock:^(NSNotification *notification) {
            Q_UNUSED(notification);
            emit accessTokenChanged();
        }
    ];

    _sdkReady = true;
}

void FacebookQml::sdkCleanUp()
{
    [[NSNotificationCenter defaultCenter] removeObserver:_FBSDKObserver];
	[static_cast<FBSDKLoginManager *>(_loginManager) release];
}

void FacebookQml::login(QStringList permissions)
{
    NSMutableArray *permissionsArray = FacebookQmlUtils::stringListToArray(permissions);

	// Check if we are already authed with the same permissions
	if ([FBSDKAccessToken currentAccessToken]) {
        if ([[NSSet setWithArray:permissionsArray] isEqualToSet:[[FBSDKAccessToken currentAccessToken] permissions]]) {
			emit loginSuccessful();
            return;
		}
	}

    [static_cast<FBSDKLoginManager *>(_loginManager) logInWithReadPermissions:permissionsArray
        fromViewController:[[UIApplication sharedApplication].keyWindow rootViewController]
        handler:^(FBSDKLoginManagerLoginResult *result, NSError *FBError) {
            if (FBError) {
                emit loginError(QString::fromNSString([FBError localizedDescription]));
            } else if (result.isCancelled) {
                emit loginCancelled();
            } else {
                emit loginSuccessful();
            }
        }
    ];
}

void FacebookQml::logout()
{
    if ([FBSDKAccessToken currentAccessToken]) {
	   [static_cast<FBSDKLoginManager *>(_loginManager) logOut];
    }
}

QString FacebookQml::accessToken()
{
	if ([FBSDKAccessToken currentAccessToken] != nil) {
		return QString::fromNSString([[FBSDKAccessToken currentAccessToken] tokenString]);
	}

    return "";
}
