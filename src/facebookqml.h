#ifndef FACEBOOKQML_H
#define FACEBOOKQML_H

#include <QQuickItem>

#if defined(Q_OS_IOS)
Q_FORWARD_DECLARE_OBJC_CLASS(FBSDKLoginManager);
#endif

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#endif

class FacebookQml : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FacebookQml)

    Q_PROPERTY(QString accessToken READ accessToken NOTIFY accessTokenChanged)

public:
    // Internal
    static void registerQmlContext();
    static FacebookQml *instance();
    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine);

#if defined(Q_OS_ANDROID)
    void handleActivityResult(int requestCode, int resultCode, jobject data);
#endif

public slots:
    // Facebook Login
    void login(QStringList permissions);
    void logout();

private:
    // Internal
    explicit FacebookQml(QQuickItem *parent = 0);
    ~FacebookQml();

    // Manage the Facebook SDK lifecycle
    void sdkSetup();
    void sdkCleanUp();
    bool _sdkReady;

    // Facebook Login
    QString accessToken();

    // Facebook Login (iOS)
#if defined(Q_OS_IOS)
    FBSDKLoginManager *_loginManager;
    NSObject *_FBSDKObserver;
#endif

    // Facebook Login (Android)
#if defined(Q_OS_ANDROID)
    QAndroidJniObject _callbackManager;
    QAndroidJniObject _facebookQml;
    static void onSuccess(JNIEnv *env, jobject thiz);
    static void onCancel(JNIEnv *env, jobject thiz);
    static void onError(JNIEnv *env, jobject thiz, jobject error);
    static void onAccessTokenChanged(JNIEnv *env, jobject thiz);
    static void onActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data);
#endif

signals:
    // Facebook Login
    void accessTokenChanged();
    void loginSuccessful();
    void loginCancelled();
    void loginError(QString error);
};

#endif // FACEBOOKQML_H
