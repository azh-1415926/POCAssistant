#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class questionpage; }
QT_END_NAMESPACE

class questionpage : public basepage
{
    Q_OBJECT

    private:
        Ui::questionpage* ui;

    protected:
        ;

    public:
        explicit questionpage(basepage* parent = nullptr);
        ~questionpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalQuestionPage();
};