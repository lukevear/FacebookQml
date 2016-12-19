#ifndef FACEBOOKLOGINANDROID_H
#define FACEBOOKLOGINANDROID_H

#include <QStringList>
#include <QAndroidJniEnvironment>

class FacebookLoginAndroid
{
public:
    static FacebookLoginAndroid *instance();
    void login(QStringList permissions);
    void logout();
    QString accessToken();

private:
    explicit FacebookLoginAndroid();

    static void JNINotifyLoginSuccess(JNIEnv *env, jobject thiz);
    static void JNINotifyLoginError(JNIEnv *env, jobject thiz, jobject error);
    static void JNINotifyLoginCancelled(JNIEnv *env, jobject thiz);
};

#endif // FACEBOOKLOGINANDROID_H
