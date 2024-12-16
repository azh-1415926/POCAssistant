#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class managementpage; }
QT_END_NAMESPACE

class managementpage : public basepage
{
    Q_OBJECT

    private:
        Ui::managementpage* ui;

    protected:
        ;

    public:
        explicit managementpage(QWidget* parent = nullptr);
        ~managementpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

    signals:
        ;

    private:
        void initalManagementpage();
};