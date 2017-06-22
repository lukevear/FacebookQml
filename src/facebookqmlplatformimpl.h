#ifndef FACEBOOKQMLPLATFORMIMPL
#define FACEBOOKQMLPLATFORMIMPL

#include <QString>
#include <QMap>
#include <QVariant>

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

class FacebookQmlPlatformImpl
{

public:
	FacebookQmlPlatformImpl();
	~FacebookQmlPlatformImpl();

#ifdef Q_OS_ANDROID
    QAndroidJniObject _callbackManager;
#endif

	// App Events Support
    void logEvent(QString event);
    void logEvent(QString event, double valueToSum);
    void logEvent(QString event, QMap<QString, QVariant> parameters);
    void logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters);

	// Facebook Login Support
    void login(QStringList permissions);
    void logout();
    QString accessToken();

private:

#ifdef Q_OS_IOS
	void *_loginManager;
#endif

#ifdef Q_OS_ANDROID
    static void JNINotifyLoginSuccess(JNIEnv *env, jobject thiz);
    static void JNINotifyLoginFailed(JNIEnv *env, jobject thiz, jobject error);
    static void JNINotifyLoginCancelled(JNIEnv *env, jobject thiz);
    static bool JNIOnActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data);
#endif

};

#endif // FACEBOOKQMLPLATFORMIMPL
