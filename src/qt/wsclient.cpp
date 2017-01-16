#include "wsclient.h"

WSClient::WSClient(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url)
{
    m_url = url;

    m_webSocket = new QWebSocket("QT App",QWebSocketProtocol::Version0);
    m_webSocket->ignoreSslErrors();
    connect(m_webSocket, &QWebSocket::connected, this, &WSClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &WSClient::onDisconnected);

    QNetworkRequest req(m_url);
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
