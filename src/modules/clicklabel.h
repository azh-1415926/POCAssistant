#pragma once

#include <QLabel>

class clicklabel : public QLabel
{
    Q_OBJECT

    private:
        ;

    public:
        explicit clicklabel(QWidget* parent = nullptr);
        ~clicklabel();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        ;

    signals:
        void clicked(const clicklabel*);
        void hover(const clicklabel*);

    private:
        #ifdef __ANDROID__
        bool touchBeginEventProcess(QEvent *e);
        bool touchUpdateEventProcess(QEvent *e);
        bool touchEndEventProcess(QEvent *e);
        #else
        bool mousePressEventProcess(QEvent* e);
        #endif
};