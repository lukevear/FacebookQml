#ifndef FACEBOOKLOGINIOS_H
#define FACEBOOKLOGINIOS_H

#include <QStringList>

class FacebookLoginiOS
{
public:
    static FacebookLoginiOS *instance();
    void login(QStringList permissions);
    void logout();
    QString accessToken();

private:
    explicit FacebookLoginiOS();
    ~FacebookLoginiOS();

    void *_FBSDKLoginManager;
    void *_accessTokenChangeObserver;

};

#endif // FACEBOOKLOGINIOS_H
