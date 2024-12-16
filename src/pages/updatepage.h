#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class updatepage; }
QT_END_NAMESPACE

class updatepage : public basepage
{
    Q_OBJECT

    private:
        Ui::updatepage* ui;

    protected:
        ;

    public:
        explicit updatepage(QWidget* parent = nullptr);
        ~updatepage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalUpdatePage();
};