#include "facebookshareandroid.h"

#include <QFile>
#include <QByteArray>

#include <QAndroidJniObject>
#include <QtAndroid>
#include <QAndroidJniEnvironment>

#include <QDebug>

void FacebookShareAndroid::imageDialog(QString filePath)
{
    QFile image(filePath);
    if (! image.open(QIODevice::ReadOnly)) {
        return;
    }

    // Read our image file into a jbyteArray
    QByteArray byteArray = image.readAll();
    QAndroidJniEnvironment env;
    jbyteArray rawImage = 0;
    int rawImageLen = byteArray.size();
    rawImage = env->NewByteArray(rawImageLen);
    env->SetByteArrayRegion(rawImage, 0, rawImageLen, (jbyte *) (const char *) byteArray.data());

    // Get a Bitmap
    QAndroidJniObject bitmap = QAndroidJniObject::callStaticObjectMethod(
        "android/graphics/BitmapFactory",
        "decodeByteArray",
        "([BII)Landroid/graphics/Bitmap;",
        rawImage,
        0,
        rawImageLen
    );

    // Created our photo to share
    QAndroidJniObject sharePhoto("com/facebook/share/model/SharePhoto$Builder");
    sharePhoto = sharePhoto.callObjectMethod(
        "setBitmap",
        "(Landroid/graphics/Bitmap;)Lcom/facebook/share/model/SharePhoto$Builder;",
        bitmap.object<jobject>()
    );
    sharePhoto = sharePhoto.callObjectMethod(
        "setUserGenerated",
        "(Z)Lcom/facebook/share/model/SharePhoto$Builder;",
        false
    );
    sharePhoto = sharePhoto.callObjectMethod(
        "build",
        "()Lcom/facebook/share/model/SharePhoto;"
    );

    // Create the content request
    QAndroidJniObject content("com/facebook/share/model/SharePhotoContent$Builder");
    content = content.callObjectMethod(
        "addPhoto",
        "(Lcom/facebook/share/model/SharePhoto;)Lcom/facebook/share/model/SharePhotoContent$Builder;",
        sharePhoto.object<jobject>()
    );
    content = content.callObjectMethod(
        "build",
        "()Lcom/facebook/share/model/SharePhotoContent;"
    );

    // Show the share dialog
    QAndroidJniObject::callStaticMethod<void>(
        "com/facebook/share/widget/ShareDialog",
        "show",
        "(Landroid/app/Activity;Lcom/facebook/share/model/ShareContent;)V",
        QtAndroid::androidActivity().object<jobject>(),
        content.object<jobject>()
    );
}
