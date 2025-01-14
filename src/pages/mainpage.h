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

        // 图标被点击，跳转至对应模块
        void selectLabel(const clicklabel* label);
        // 切换至管理员界面
        void switchTo(int role);

    signals:
        void selectPage(int i);

    private:
        void initalMainPage();
};