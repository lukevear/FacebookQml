#include "facebookshareios.h"

#import "UIKit/UIKit.h"
#include <FBSDKShareKit/FBSDKShareKit.h>

#include <QFile>
#include <QByteArray>

void FacebookShareiOS::imageDialog(QString filePath)
{
    QFile image(filePath);
    if (! image.open(QIODevice::ReadOnly)) {
        return;
    }

    FBSDKSharePhoto *photo = [[FBSDKSharePhoto alloc] init];
    photo.image = [UIImage imageWithData:image.readAll().toRawNSData()];
    photo.userGenerated = NO;

    FBSDKSharePhotoContent *content = [[FBSDKSharePhotoContent alloc] init];
    content.photos = @[photo];

    [FBSDKShareDialog showFromViewController:[[UIApplication sharedApplication].keyWindow rootViewController]
        withContent:content
        delegate:nil
    ];
}
