#ifndef FACEBOOKSHARE_H
#define FACEBOOKSHARE_H

#include <QQuickItem>

class FacebookShare : public QQuickItem
{
    Q_OBJECT
    Q_DISABLE_COPY(FacebookShare)

public:
    // Used to access the singleton
    static FacebookShare *instance();
    static QObject *instance(QQmlEngine *engine, QJSEngine *scriptEngine);

    Q_INVOKABLE void imageDialog(QString filePath);

private:
    explicit FacebookShare(QQuickItem *parent = 0);

};

#endif // FACEBOOKSHARE_H
