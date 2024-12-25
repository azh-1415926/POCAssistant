#pragma once

#include "base.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>

SINGLETONE_P(httpManager,QNetworkAccessManager)
SINGLETONE(userId,QString)
SINGLETONE(userRole,int)
SINGLETONE(tokenOfAdmin,QString)