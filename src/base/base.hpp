#pragma once

#include <QString>
#include <QPixmap>

#include <QScopedPointer>
#include <QNetworkAccessManager>

struct StatusOfPage
{
    QString currPage;
    QPixmap currIcon;
    QString currInfo;
};

#ifdef MANAGER_IMPL
    QScopedPointer<QNetworkAccessManager> httpManager(new QNetworkAccessManager);
#else
    extern QScopedPointer<QNetworkAccessManager> httpManager;
#endif