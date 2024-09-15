#pragma once

#include "basepage.h"

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
        explicit coursepage(basepage* parent = nullptr);
        ~coursepage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalCoursePage();
};