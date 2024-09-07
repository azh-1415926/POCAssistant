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
        virtual ~basepage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void select();
        virtual void resetPage();

        void setStatus(const QString& currPage,const QString& currIcon,const QString& currInfo="");
        void setStatus(const StatusOfPage& status);

        void back();

    signals:
        void refreshStatus(const StatusOfPage& status);

    private:
        void initalBasePage();
};