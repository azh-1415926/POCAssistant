#include "collectbutton.h"

#ifndef __ANDROID__
    #include <QMouseEvent>
#else
    #include <QTouchEvent>
#endif

collectbutton::collectbutton(QWidget *parent, const QString &collect, const QString &uncollect)
    : QPushButton(parent)
    , textOfCollect(collect), textOfUncollect(uncollect)
{
    installEventFilter(this);
    #ifdef __ANDROID__
        this->setAttribute(Qt::WA_AcceptTouchEvents);
    #endif
}

collectbutton::~collectbutton()
{
}

/* 设置按钮状态为被收藏，发送 collected 信号 */
void collectbutton::setCollect()
{
    this->setText(textOfCollect);
    emit collected();
}

bool collectbutton::eventFilter(QObject *obj, QEvent *e)
{
    if(obj==this)
    {
        switch (e->type())
        {
        #ifdef __ANDROID__
        case QEvent::TouchBegin:
            return touchBeginEventProcess(e);
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

/* 设置按钮状态为取消收藏，发送 uncollected 信号 */
void collectbutton::setUncollect()
{
    this->setText(textOfUncollect);
    emit uncollected();
}

/* 设置按钮状态为被收藏，不发送信号 */
void collectbutton::setCollectNoSignal()
{
    this->setText(textOfCollect);
}

/* 设置按钮状态为取消收藏，不发送信号 */
void collectbutton::setUncollectNoSignal()
{
    this->setText(textOfUncollect);
}

#ifdef __ANDROID__

/* 触屏事件，每点击一次，都会触发收藏或取消收藏，并发送 collected、uncollected 信号 */
bool collectbutton::touchBeginEventProcess(QEvent *e)
{
    QTouchEvent *touchEvent = static_cast<QTouchEvent*>(e);
    if(this->text()==textOfCollect)
    {
        this->setText(textOfUncollect);
        emit uncollected();
    }
    else
    {
        this->setText(textOfCollect);
        emit collected();
    }
    touchEvent->accept();
    return true;
}

#else

/* 鼠标点击事件，每点击一次，都会触发收藏或取消收藏，并发送 collected、uncollected 信号 */
bool collectbutton::mousePressEventProcess(QEvent* e)
{
    if(this->text()==textOfCollect)
    {
        this->setText(textOfUncollect);
        emit uncollected();
    }
    else
    {
        this->setText(textOfCollect);
        emit collected();
    }
    e->accept();
    return true;
}

#endif