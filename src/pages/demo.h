#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class demo; }
QT_END_NAMESPACE

class demo : public basepage
{
    Q_OBJECT

    private:
        Ui::demo* ui;

    protected:
        ;

    public:
        explicit demo(basepage* parent = nullptr);
        ~demo();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initaldemo();
};