#pragma once

#include <QWidget>

#include "basepage.h"
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
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void showLoginPage();
        void hideLoginPage();

        void setPageStatus(const StatusOfPage& status);
        void goToBack();
        void selectPage(int i);

        void getInfo(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalWidget();
        void initalTopPages();
        void initalAllPages();
        void initalMainPage();
};