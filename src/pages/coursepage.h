#pragma once

#include "basepage.h"

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class coursepage; }
QT_END_NAMESPACE

class coursepage : public basepage
{
    Q_OBJECT

    private:
        Ui::coursepage* ui;

    protected:
        ;

    public:
        explicit coursepage(QWidget* parent = nullptr);
        ~coursepage();

        // 获取页面中课程大纲内容
        QString getOutLine(QTreeWidget* w);

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();
        virtual void selectedPage();

        void back();

        // 设置页面的课程大纲
        void setOutLine(QNetworkReply* reply);
        // 设置当前章节课程展示的内容
        void setContent(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalCoursePage();
};