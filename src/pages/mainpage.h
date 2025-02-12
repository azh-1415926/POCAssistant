#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class mainpage; }
QT_END_NAMESPACE

class clicklabel;

// 主界面页面
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
        void switchTo(int role);

    signals:
        void selectPage(int i);

    private:
        void initalMainPage();
};