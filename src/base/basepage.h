#pragma once

#include <QWidget>
#include <QNetworkReply>

#include "base.hpp"
#include "global.hpp"
#include "constant.h"

// 页面的基本信息
struct StatusOfPage
{
    QString currPage;
    QPixmap currIcon;
    QString currInfo;
};

// 用户类型
enum class UserRole : int
{
    STUDENT=0,
    TEACHER=1,
    ADMIN=2
};

// 页面基类，方便切换页面时与主界面按钮交互
class basepage : public QWidget
{
    Q_OBJECT

    protected:
        StatusOfPage m_Status;

    public:
        explicit basepage(QWidget* parent = nullptr);
        basepage(const QString& currPage,QWidget* parent = nullptr);
        virtual ~basepage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        // 切换至该页面时由主界面执行该槽函数
        void select();
        // 重置页面
        virtual void resetPage();
        // 在此编写页面被选中时的初始化操作
        virtual void selectedPage();

        // 设置页面信息
        void setStatus(const QString& currPage,const QString& currIcon,const QString& currInfo="");
        void setStatus(const StatusOfPage& status);
        void setName(const QString& str) { m_Status.currPage=str; }
        void setIcon(const QString& str) { m_Status.currIcon.load(str); }
        void setInfo(const QString& str) { m_Status.currIcon=str; }

        // 页面后退一页
        virtual void back();

    signals:
        // 用于与登陆页面交互
        void logon();
        void logoff();

        // 与主界面交互，将自身状态发送给主界面
        void refreshStatus(const StatusOfPage& status);

    private:
        void initalBasePage();
};