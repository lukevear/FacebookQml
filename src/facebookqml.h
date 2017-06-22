#ifndef FACEBOOK_H
#define FACEBOOK_H

#include <QQuickItem>

#include "facebookqmlplatformimpl.h"

class FacebookQml : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FacebookQml)

public:
    // Used to access the singleton
    static void registerQmlContext();
    static FacebookQml *instance();
    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine);

    // Use to access the platform specific implementation
    QSharedPointer<FacebookQmlPlatformImpl> _platformImpl;

    // App Events Support
    Q_INVOKABLE void logEvent(QString event);
    Q_INVOKABLE void logEvent(QString event, double valueToSum);
    Q_INVOKABLE void logEvent(QString event, QMap<QString, QVariant> parameters);
    Q_INVOKABLE void logEvent(QString event, double valueToSum, QMap<QString, QVariant> parameters);

    // Facebook Login Support
    Q_INVOKABLE void login(QStringList permissions);
    Q_INVOKABLE void logout();
    Q_INVOKABLE QString accessToken();

private:
    // Internal
    explicit FacebookQml(QQuickItem *parent = 0);

signals:
    // Facebook Login Support
    void loginSuccessful();
    void loginCancelled();
    void loginFailed(QString error);
};

#endif // FACEBOOK_H
