#pragma once

#include <QWidget>

class widget : public QWidget
{
    Q_OBJECT

    private:
        ;

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