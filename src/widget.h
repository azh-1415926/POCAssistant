#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class widget; }
QT_END_NAMESPACE

class widget : public QWidget
{
    Q_OBJECT

    private:
        Ui::widget* ui;

    public:
        explicit widget(QWidget* parent = nullptr);
        ~widget();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        ;

    private:
        ;
};