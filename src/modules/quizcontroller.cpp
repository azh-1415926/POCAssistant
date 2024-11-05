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
}

void quizcontroller::bind(quizmodel *m)
{
    model=m;

    connect(model,&quizmodel::dataChanged,this,&quizcontroller::InitalView);
}

void quizcontroller::unbind()
{
    if(view)
    {
        disconnect(view,&quizview::prevQuiz,this,&quizcontroller::goPrevQuiz);
        disconnect(view,&quizview::nextQuiz,this,&quizcontroller::goNextQuiz);

        view=nullptr;
    }

    if(model)
    {
        disconnect(model,&quizmodel::dataChanged,this,&quizcontroller::InitalView);

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

void quizcontroller::InitalView()
{
    if(view==nullptr)
        return;

    updateQuiz(0);
    view->setSum(model->count());
}

void quizcontroller::initalController()
{
}

void quizcontroller::updateQuiz(int index)
{
    view->setIndex(index);
    view->updateQuiz(model->get(index));
}
