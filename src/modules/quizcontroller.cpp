#include "quizcontroller.h"

quizcontroller::quizcontroller(QObject *parent)
    : view(nullptr), model(nullptr)
    , m_Index(0)
{
}

quizcontroller::~quizcontroller()
{
    unbind();
}

void quizcontroller::bind(quizview *v)
{
    view=v;
    
    connect(view,&quizview::prevQuiz,this,&quizcontroller::goPrevQuiz);
    connect(view,&quizview::nextQuiz,this,&quizcontroller::goNextQuiz);
    connect(view,&quizview::collectQuestion,this,&quizcontroller::collectCurrQuiz);
    connect(view,&quizview::uncollectQuestion,this,&quizcontroller::uncollectCurrQuiz);
}

void quizcontroller::bind(quizmodel *m)
{
    model=m;

    connect(model,&quizmodel::dataChanged,this,&quizcontroller::initalView);
}

void quizcontroller::unbind()
{
    if(view)
    {
        disconnect(view,&quizview::prevQuiz,this,&quizcontroller::goPrevQuiz);
        disconnect(view,&quizview::nextQuiz,this,&quizcontroller::goNextQuiz);
        disconnect(view,&quizview::collectQuestion,this,&quizcontroller::collectCurrQuiz);
        disconnect(view,&quizview::uncollectQuestion,this,&quizcontroller::uncollectCurrQuiz);

        view=nullptr;
    }

    if(model)
    {
        disconnect(model,&quizmodel::dataChanged,this,&quizcontroller::initalView);

        model=nullptr;
    }
}

void quizcontroller::goPrevQuiz()
{
    if(model==nullptr||m_Index<=0)
        return;

    updateQuiz(--this->m_Index);
}

void quizcontroller::goNextQuiz()
{
    if(model==nullptr||m_Index>=model->count()-1)
        return;

    updateQuiz(++this->m_Index);
}

void quizcontroller::initalView()
{
    if(view==nullptr)
        return;

    updateQuiz(0);
}

void quizcontroller::collectCurrQuiz()
{
    if(model==nullptr)
        return;

    model->setCollected(m_Index,true);
}

void quizcontroller::uncollectCurrQuiz()
{
    if(model==nullptr)
        return;

    model->setCollected(m_Index,false);
}

void quizcontroller::initalController()
{
}

void quizcontroller::updateQuiz(int index)
{
    if(model->count()==0)
    {
        view->reset();
        return;
    }
            
    if(index>=model->count())
    {
        return;
    }
    view->setIndex(index);
    view->setSum(model->count());
    view->setCollect(model->getCollectedStatus(index));
    view->updateQuiz(model->get(index));
}
