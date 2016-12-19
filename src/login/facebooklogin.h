#ifndef FACEBOOKLOGIN_H
#define FACEBOOKLOGIN_H

#include <QQuickItem>

class FacebookLogin : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FacebookLogin)
    Q_PROPERTY(QString accessToken READ accessToken NOTIFY accessTokenChanged)

public:
    // Used to access the singleton
    static FacebookLogin *instance();
    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine);

    // Login/Logout
    Q_INVOKABLE void login(QStringList permissions);
    Q_INVOKABLE void logout();

private:
    explicit FacebookLogin(QQuickItem *parent = 0);

    // Used to retrieve the current Facebook access token
    QString accessToken();

signals:
    void accessTokenChanged();
    void error(QString message);
    void cancelled();
    void authenticated();
    void loggedOut();

};

#endif // FACEBOOKLOGIN_H
