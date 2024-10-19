#pragma once

#include <QWidget>

#include "base.hpp"

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
        void select();
        virtual void resetPage();

        void setStatus(const QString& currPage,const QString& currIcon,const QString& currInfo="");
        void setStatus(const StatusOfPage& status);
        void setName(const QString& str) { m_Status.currPage=str; }
        void setIcon(const QString& str) { m_Status.currIcon.load(str); }
        void setInfo(const QString& str) { m_Status.currIcon=str; }

        virtual void back();

    signals:
        void logon();
        void logoff();

        void refreshStatus(const StatusOfPage& status);

    private:
        void initalBasePage();
};