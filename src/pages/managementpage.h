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
        
        QString getUrlByOperation(operationOfManagement op);

        // 分别对应用户管理、班级管理、查询
        void operationOfUser(QNetworkReply* reply);
        void operationOfClass(QNetworkReply* reply);
        void operationOfSearch(QNetworkReply* reply);
        void loadUnallocatedStudent(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalManagementPage();

        void loadUserInfo(bool needToLoad,bool needToClearId=false);
        void loadClassInfo(bool needToLoad,bool needToClearId=false);

        void lockUserInfo(bool shouldLock);
        void lockClassInfo(bool shouldLock);

        void clearUserInfo();
        void clearClassInfo();

        void postByOperationWithData(int funcId,const operationOfManagement& op,const QJsonObject& obj);
        void postByCurrOperationWithData(int funId,const QJsonObject& obj);
};