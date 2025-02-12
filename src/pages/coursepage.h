#pragma once

#include "basepage.h"

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class coursepage; }
QT_END_NAMESPACE

// 在线课程页面，建议对普通用户展示，对管理员隐藏
class coursepage : public basepage
{
    Q_OBJECT

    private:
        Ui::coursepage* ui;

        // 记录当前章、下标
        int m_CurrChapter;
        int m_CurrSection;

    protected:
        ;

    public:
        explicit coursepage(QWidget* parent = nullptr);
        ~coursepage();

        QString getOutLine(QTreeWidget* w);
        QPair<QPair<int,int>,QString> getCurrCourse();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();
        virtual void selectedPage();

        void back();

        void setOutLine(QNetworkReply* reply);
        void setContent(QNetworkReply* reply);
        void setEditable(bool status);

    signals:
        ;

    private:
        void initalCoursePage();
};