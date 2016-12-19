#ifndef FACEBOOKQML_H
#define FACEBOOKQML_H

#include <QQuickItem>

#if defined(Q_OS_ANDROID)
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#endif

class FacebookQml : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FacebookQml)

public:
    // Used to access the singleton
    static void registerQmlContext();
    static FacebookQml *instance();
    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine);

#if defined(Q_OS_ANDROID)
    QAndroidJniObject getFBCallbackManager();
#endif

private:
    explicit FacebookQml(QQuickItem *parent = 0);

#if defined(Q_OS_ANDROID)
    // Used to handle facebook callbacks
    QAndroidJniObject _FBCallbackManager;

    // Used to handle activity results
    static bool JNIOnActivityResult(JNIEnv *env, jobject thiz, int requestCode, int resultCode, jobject data);
#endif
};

#endif // FACEBOOKQML_H
