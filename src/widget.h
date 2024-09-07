#pragma once

#include <QWidget>

#include "base.hpp"
#include "clicklabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class widget; }
QT_END_NAMESPACE

class widget : public QWidget
{
    Q_OBJECT

    private:
        Ui::widget* ui;
        bool m_FlagOfInit;

    public:
        explicit widget(QWidget* parent = nullptr);
        ~widget();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setPageStatus(const StatusOfPage& status);
        void goToBack();
        void selectPage(int i);
        void selectPage(const clicklabel* label);

    private:
        void initalWidget();
        void initalMainPage();
};