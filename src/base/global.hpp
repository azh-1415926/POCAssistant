#pragma once

#include "base.hpp"
#include "constant.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>

// 用于发送网络请求
SINGLETONE_P(httpManager,QNetworkAccessManager)
// 服务器 url
SINGLETONE_WITH_VALUE(urlOfServer,QString,"http://" \
    SERVER_IP \
    ":" \
    SERVER_PORT_S \
)
// 用户 id
SINGLETONE(userId,QString)
// 用户类型
SINGLETONE(userRole,int)
// 管理员的 token
SINGLETONE(tokenOfAdmin,QString)

// 简化变量
#define HTTP_MANAGER SINGLETONE_VALUE(httpManager)
#define URL_OF_SERVER SINGLETONE_VALUE(urlOfServer)