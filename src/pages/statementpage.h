#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class statementpage; }
QT_END_NAMESPACE

class statementpage : public basepage
{
    Q_OBJECT

    private:
        Ui::statementpage* ui;

    protected:
        ;

    public:
        explicit statementpage(QWidget* parent = nullptr);
        ~statementpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalStatementPage();
};