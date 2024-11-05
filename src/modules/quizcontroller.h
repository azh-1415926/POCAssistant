#pragma once

#include <QObject>

#include "quizview.h"
#include "quizmodel.h"

class quizcontroller : public QObject
{
    Q_OBJECT

    private:
        quizview* view;
        quizmodel* model;

        int m_Index;

    public:
        explicit quizcontroller(QObject* parent = nullptr);
        virtual ~quizcontroller();
        
        ;

    protected:
        ;

    public slots:
        void bind(quizview* v);
        void bind(quizmodel* m);

        void unbind();

        void goPrevQuiz();
        void goNextQuiz();

        void InitalView();

    signals:
        ;

    private:
        void initalController();

        void updateQuiz(int index);
};