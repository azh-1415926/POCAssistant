#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class loginpage; }
QT_END_NAMESPACE

class loginpage : public basepage
{
    Q_OBJECT

    private:
        Ui::loginpage* ui;

    protected:
        ;

    public:
        explicit loginpage(QWidget* parent = nullptr);
        ~loginpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalLoginPage();
};