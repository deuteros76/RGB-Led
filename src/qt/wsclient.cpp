#include "wsclient.h"

WSClient::WSClient(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    m_url = url;
    m_mDNS = qMDNS::getInstance();

    m_webSocket = new QWebSocket("QT App",QWebSocketProtocol::Version0);
    m_webSocket->ignoreSslErrors();
    connect(m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WSClient::onDisconnected);
    connect(m_mDNS, &qMDNS::hostFound,this,&WSClient::connectToServer);

    m_mDNS->lookup (m_url.toString());
}

void WSClient::connectToServer(const QHostInfo& info){
    QString url = "ws://"+info.addresses().first().toString()+":81/";
    qDebug() << "Connecting to "<< url;
    QNetworkRequest req(url);
    req.setRawHeader("Connection","keep-alive, Upgrade");
    m_webSocket->open(req);
}

void WSClient::onConnected()
{
    qDebug() << "WebSocket connected";
    connect(m_webSocket, &QWebSocket::textMessageReceived,this, &WSClient::onTextMessageReceived);
    m_webSocket->sendTextMessage(QStringLiteral("#ff0000"));
}

void WSClient::onColorChanged(QString color)
{
    m_webSocket->sendTextMessage(color);
    qDebug() << "Color changed to " << color;
}


void WSClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;

}

void WSClient::onDisconnected()
{
    qDebug() << "Disconnected " << m_webSocket->state();
    qDebug() << m_webSocket->closeReason();
}
