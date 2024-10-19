#pragma once

#include <QString>
#include <QPixmap>

#include <QScopedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>

struct StatusOfPage
{
    QString currPage;
    QPixmap currIcon;
    QString currInfo;
};

class httpManager : QObject
{
    public:
        static httpManager& getInstance()
        {
            static httpManager instance;
            return instance;
        }
        QNetworkAccessManager* getManger()
        {
            if(m_Manger==nullptr)
                m_Manger=new QNetworkAccessManager(this);

            return m_Manger;
        }

    private:
        explicit httpManager(QObject* parent=nullptr): QObject(parent), m_Manger(nullptr) { }

        QNetworkAccessManager* m_Manger;
};

class httpCookie : QObject
{
    public:
        static httpCookie& getInstance()
        {
            static httpCookie instance;
            return instance;
        }
        QString cookie() { return m_Cookie; }

        void setCookie(QString c) { m_Cookie=c; }

    private:
        QString m_Cookie;
};