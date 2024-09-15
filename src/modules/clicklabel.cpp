#include "clicklabel.h"

#ifndef __ANDROID__
#include <QMouseEvent>
#else
#include <QTouchEvent>
#endif

clicklabel::clicklabel(QWidget *parent)
    : QLabel(parent)
{
    this->installEventFilter(this);
    #ifdef __ANDROID__
    this->setAttribute(Qt::WA_AcceptTouchEvents);
    #endif
}

clicklabel::~clicklabel()
{
}

bool clicklabel::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==this)
    {
        switch (e->type())
        {
        #ifdef __ANDROID__
        case QEvent::TouchBegin:
            return touchBeginEventProcess(e);
        case QEvent::TouchUpdate:
            return touchUpdateEventProcess(e);
        case QEvent::TouchEnd:
            return touchEndEventProcess(e);
        #else
        case QEvent::MouseButtonPress:
            return mousePressEventProcess(e);
        #endif
        default:
            break;
        }
    }
    return false;
}

#ifdef __ANDROID__

bool clicklabel::touchBeginEventProcess(QEvent *e)
{
    QTouchEvent *touchEvent = static_cast<QTouchEvent*>(e);
    emit hover(this);
    emit clicked(this);
    touchEvent->accept();
    return true;
}

bool clicklabel::touchUpdateEventProcess(QEvent *e)
{
    QTouchEvent *touchEvent = static_cast<QTouchEvent*>(e);
    touchEvent->accept();
    return true;
}

bool clicklabel::touchEndEventProcess(QEvent *e)
{
    QTouchEvent *touchEvent = static_cast<QTouchEvent*>(e);
    touchEvent->accept();
    return true;
}

#else

bool clicklabel::mousePressEventProcess(QEvent *e)
{
    emit clicked(this);
    e->accept();
    return true;
}

#endif