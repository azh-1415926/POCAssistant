#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class managementpage; }
QT_END_NAMESPACE

enum class operationOfManagement
{
    ADD_USER=0,REMOVE_USER,ALTER_USER,
    ADD_CLASS,REMOVE_CLASS,ALTER_CLASS,CLASS_ALLOC,
    USER_INFO,CLASS_INFO,
    USER_SEARCH,CLASS_SERACH
};

class managementpage : public basepage
{
    Q_OBJECT

    private:
        Ui::managementpage* ui;
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
        
        // 获取当前操作的后端请求 url
        QString getUrlByOperation(operationOfManagement op);

        // 分别对应用户管理、班级管理、查询
        void operationOfUser(QNetworkReply* reply);
        void operationOfClass(QNetworkReply* reply);
        void operationOfSearch(QNetworkReply* reply);

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