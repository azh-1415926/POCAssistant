#pragma once

#include "basepage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class quizpage; }
QT_END_NAMESPACE

class quizmodel;

class quizpage : public basepage
{
    Q_OBJECT

    private:
        Ui::quizpage* ui;
        quizmodel* modelOfTest;

    protected:
        ;

    public:
        explicit quizpage(QWidget* parent = nullptr);
        ~quizpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

        void updateTest(QNetworkReply* reply);
        void updateCollection(QNetworkReply* reply);
        void updateWrong(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalQuizPage();
};