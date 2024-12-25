#pragma once

#include "base.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>

// 用于发送网络请求
SINGLETONE_P(httpManager,QNetworkAccessManager)
// 用户 id
SINGLETONE(userId,QString)
// 用户类型
SINGLETONE(userRole,int)
// 管理员的 token
SINGLETONE(tokenOfAdmin,QString)