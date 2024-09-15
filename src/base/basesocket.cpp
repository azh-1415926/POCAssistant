#include "basesocket.h"

#include "constant.h"
#include "jsonFile.hpp"

// #include <QWebSocket>

_basesocket::_basesocket(const QString& ip,int port,QObject* parent)
    : QObject(parent), m_Ip(ip), m_Port(port), isConnected(false)
{
    s=new QTcpSocket();
    
    connect(s,&QTcpSocket::connected,this,[=]()
    {
        qDebug()<<"Connect to server！";
        isConnected=true;
    });

    connect(s,&QTcpSocket::disconnected,this,[=]()
    {
        qDebug()<<"DisConnected to server！";
        isConnected=false;
    });

    // open();
    // QWebSocket* client=new QWebSocket;
    // client->open(QUrl("ws://127.0.0.1:8848/chat"));
    
    // connect(client,&QWebSocket::connected,this,[=]()
    // {
    //     client->sendTextMessage("hello");

    //     jsonFile json;
    //     json.add("account","123");
    //     json.add("password","123");
    //     qDebug()<<"json:"<<json.toString();

    //     client->sendTextMessage(json.toString());
    // });

    // connect(client,&QWebSocket::textMessageReceived,this,[=](const QString& str)
    // {
    //     qDebug()<<"recv:"<<str;
    // });    
}

_basesocket::~_basesocket()
{
    s->close();
    delete s;
}

void _basesocket::open()
{
    s->connectToHost(m_Ip,m_Port);
}

void _basesocket::send(const QString& request)
{
    if(!isConnected)
    {
        open();

        s->waitForConnected(3000);

        emit timeout();

        return;
    }

    s->write(request.toStdString().c_str(),request.size());
}

void _basesocket::recv(QString &response)
{
    if(!isConnected)
    {
        open();

        s->waitForConnected(3000);

        emit timeout();

        return;
    }

    response=s->readAll();
}

basesocket::basesocket(QObject *parent)
    : QObject(parent)
{
}

basesocket::~basesocket()
{
    ;
}

_basesocket *basesocket::getInstance()
{
    static _basesocket* socket=nullptr;

    if(socket==nullptr)
    {
        socket=new _basesocket(RemoteIp,RemotePort);
        // connect(socket,&_basesocket::logoff,this,[=]()
        // {
        //     emit logoff();
        // });
    }

    return socket;
}