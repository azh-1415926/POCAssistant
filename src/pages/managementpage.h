#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class managementpage; }
QT_END_NAMESPACE

// 管理页面子操作的枚举
enum class operationOfManagement
{
    // 用户管理模块
    ADD_USER=0,REMOVE_USER,ALTER_USER,
    // 班级管理模块
    ADD_CLASS,REMOVE_CLASS,ALTER_CLASS,CLASS_ALLOC,
    // 查询模块
    USER_SEARCH,CLASS_SERACH,
    // 批量处理模块
    USER_BATCH,CLASS_BATCH,
    // 额外模块，仅请求用于展示的数据
    USER_INFO,CLASS_INFO,UNALLOC_USER
};

// 在线管理页面，建议对管理员显示，对普通用户隐藏
class managementpage : public basepage
{
    Q_OBJECT

    private:
        Ui::managementpage* ui;

        // 用户、班级信息展示框是否被锁定
        bool userInfoIsLock;
        bool classInfoIsLock;

    protected:
        ;

    public:
        explicit managementpage(QWidget* parent = nullptr);
        ~managementpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();
        
        // 获取当前操作的请求 url 地址
        QString getUrlByOperation(operationOfManagement op);

        // 分别对应用户管理、班级管理、查询、批处理
        void operationOfUser(QNetworkReply* reply);
        void operationOfClass(QNetworkReply* reply);
        void operationOfSearch(QNetworkReply* reply);
        void operationOfBatch(QNetworkReply* reply);

        // 加载未分配班级的学生列表
        void loadUnallocatedStudent(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalManagementPage();

        // 读取用户信息，needToLoad 更新框内信息，needToClearId 是否清除id
        void loadUserInfo(bool needToLoad,bool needToClearId=false);
        // 读取班级信息，needToLoad 更新框内信息，needToClearId 是否清除id
        void loadClassInfo(bool needToLoad,bool needToClearId=false);

        // 锁定用户信息，shouldLock 是否锁住信息框
        void lockUserInfo(bool shouldLock);
        // 锁定班级信息，shouldLock 是否锁住信息框
        void lockClassInfo(bool shouldLock);

        // 清除用户信息
        void clearUserInfo();
        // 清除班级信息
        void clearClassInfo();

        // 使用指定的响应回调函数的 funcId 绑定指定请求的 op，发送指定数据 obj
        void postByOperationWithData(int funcId,const operationOfManagement& op,const QJsonObject& obj);
        // 使用指定的响应回调函数的 funcId 绑定指定当前请求操作,发送指定数据 obj
        void postByCurrOperationWithData(int funId,const QJsonObject& obj);
};