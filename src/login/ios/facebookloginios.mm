#include "facebookloginios.h"

#include <FBSDKCoreKit/FBSDKCoreKit.h>
#include <FBSDKLoginKit/FBSDKLoginKit.h>
#include "facebooklogin.h"

static FacebookLoginiOS *_instance = 0;

FacebookLoginiOS::FacebookLoginiOS()
{
	_FBSDKLoginManager = [[FBSDKLoginManager alloc] init];
    
    _accessTokenChangeObserver = [[NSNotificationCenter defaultCenter]
        addObserverForName:FBSDKAccessTokenDidChangeNotification
        object:nil
        queue:nil
        usingBlock:^(NSNotification *notification) {
            Q_UNUSED(notification);

            emit FacebookLogin::instance()->accessTokenChanged();
        }
    ];
}

FacebookLoginiOS::~FacebookLoginiOS()
{
	[static_cast<FBSDKLoginManager *>(_FBSDKLoginManager) release];

    [[NSNotificationCenter defaultCenter]
        removeObserver:static_cast<NSObject *>(_accessTokenChangeObserver)
    ];
}

FacebookLoginiOS *FacebookLoginiOS::instance()
{
    if (_instance == 0) {
        _instance = new FacebookLoginiOS();
    }

    return _instance;
}

void FacebookLoginiOS::login(QStringList permissions)
{
	// Build our array of permissions
	NSMutableArray *permissionsList = [[NSMutableArray alloc] init];
	for (int i = 0; i < permissions.size(); ++i) {
		[permissionsList addObject:permissions.at(i).toNSString()];
	}
	
    // Perform the login request using the FBSDKLoginManager
    [static_cast<FBSDKLoginManager *>(_FBSDKLoginManager) logInWithReadPermissions: permissionsList
        fromViewController:[[UIApplication sharedApplication].keyWindow rootViewController]
        handler:^(FBSDKLoginManagerLoginResult *result, NSError *FBError) {
            if (FBError) {
                emit FacebookLogin::instance()->error(QString::fromNSString([FBError localizedDescription]));
            } else if (result.isCancelled) {
                emit FacebookLogin::instance()->cancelled();
            } else {
                emit FacebookLogin::instance()->authenticated();
            }
        }
    ];
}

void FacebookLoginiOS::logout()
{
	[static_cast<FBSDKLoginManager *>(_FBSDKLoginManager) logOut];
}

QString FacebookLoginiOS::accessToken()
{
	if ([FBSDKAccessToken currentAccessToken] != nil) {
		return QString::fromNSString([[FBSDKAccessToken currentAccessToken] tokenString]);
	}

	return "";
}
