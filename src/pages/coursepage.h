#pragma once

#include "basepage.h"

#include <QTreeWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class coursepage; }
QT_END_NAMESPACE

class coursepage : public basepage
{
    Q_OBJECT

    private:
        Ui::coursepage* ui;

    protected:
        ;

    public:
        explicit coursepage(QWidget* parent = nullptr);
        ~coursepage();

        QString getOutLine(QTreeWidget* w);

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

        void setOutLine(QString str);

    signals:
        ;

    private:
        void initalCoursePage();
};