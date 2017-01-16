#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QHostInfo>
#include <qnetworkrequest.h>

class WSClient : public QObject
{
    Q_OBJECT
public:
    explicit WSClient(const QUrl &url,QObject *parent = 0);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);
    void onColorChanged(QString color);
    void onDisconnected();
private:
    QWebSocket *m_webSocket;
    QUrl m_url;
};

#endif // WSCLIENT_H
