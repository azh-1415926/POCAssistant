#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class managementpage; }
QT_END_NAMESPACE

class managementpage : public basepage
{
    Q_OBJECT

    private:
        Ui::managementpage* ui;

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
        QString getUrlByOperation(int index);

        // 分别对应用户管理、班级管理、查询
        void operationOfUser(QNetworkReply* reply);
        void operationOfClass(QNetworkReply* reply);
        void operationOfSearch(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalManagementPage();
};