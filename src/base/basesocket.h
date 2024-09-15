#pragma once

#include <QTcpSocket>

class _basesocket : QObject
{
    Q_OBJECT

    private:
        QString m_Ip;
        int m_Port;
        QTcpSocket* s;

        bool isConnected;

    public:
        explicit _basesocket(const QString& ip,int port,QObject* parent = nullptr);
        ~_basesocket();

    public slots:
        void open();

        void send(const QString& request);
        void recv(QString& response);

    signals:
        void logoff();
        void timeout();
};

class basesocket : QObject
{
    Q_OBJECT

    private:
        ;

    public:
        explicit basesocket(QObject* parent = nullptr);
        ~basesocket();

        _basesocket* getInstance();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        ;

    signals:
        void logoff();

    private:
        ;
};