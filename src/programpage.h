#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class programpage; }
QT_END_NAMESPACE

class programpage : public basepage
{
    Q_OBJECT

    private:
        Ui::programpage* ui;

    protected:
        ;

    public:
        explicit programpage(basepage* parent = nullptr);
        ~programpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalProgramPage();
};