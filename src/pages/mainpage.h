#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainpage; }
QT_END_NAMESPACE

class clicklabel;

class mainpage : public basepage
{
    Q_OBJECT

    private:
        Ui::mainpage* ui;

    protected:
        ;

    public:
        explicit mainpage(QWidget* parent = nullptr);
        ~mainpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();
        void selectLabel(const clicklabel* label);

        void switchToAdmin(bool state);

    signals:
        void selectPage(int i);

    private:
        void initalmainpage();
};