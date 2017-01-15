#ifndef WSCLIENT_H
#define WSCLIENT_H

#include <QObject>
#include <QtWebSockets/QWebSocket>
#include <QHostInfo>
#include <qnetworkrequest.h>
#include "qMDNS/qMDNS.h"

class WSClient : public QObject
{
    Q_OBJECT
public:
    explicit WSClient(const QUrl &url,QObject *parent = 0);

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void connectToServer(const QHostInfo& info);
    void onConnected();
    void onTextMessageReceived(QString message);
    void onColorChanged(QString color);
    void onDisconnected();
private:
    QWebSocket *m_webSocket;
    QUrl m_url;
    qMDNS* m_mDNS;
};

#endif // WSCLIENT_H
