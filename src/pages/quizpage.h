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

        // 章节测试题目数据 model
        quizmodel* modelOfTest;
        // 章节测试收藏题目数据 model
        quizmodel* modelOfCollectedTest;
        // 错题回顾
        quizmodel* modelOfWrongQuiz;

        // Teacher
        QList<QPair<QString,QString>> classList;
        QList<QPair<QString,QString>> studentInfo;
        QList<QPair<QString,QString>> experimentInfo;
        QList<QPair<QString,QString>> experimentContent;

    protected:
        ;

    public:
        explicit quizpage(QWidget* parent = nullptr);
        ~quizpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();
        virtual void selectedPage();

        void back();

        void updateChapter(QNetworkReply* reply);
        // 更新子页面
        void updateTest(QNetworkReply* reply);
        void updateCollection(QNetworkReply* reply);
        void updateWrong(QNetworkReply* reply);

        void getCollectedStatus(QNetworkReply* reply);
        void getUncollectedStatus(QNetworkReply* reply);
        void getAnswer(QNetworkReply* reply);
        void getClass(QNetworkReply* reply);
        void getStudent(QNetworkReply* reply);
        void getExperiment(QNetworkReply* reply);
        void getSubmitScoreStatus(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalQuizPage();
        void requestChapter();
        void requestTestQuiz();
        void requestCollectedQuiz();
        void requestAnswer();

        void requestClass();
        void requestStudent(const QString& classId);
        void requestExperiment(const QString& studentId);
        void submitScore(const QString& studentId,const QString& experimentId,int score);

        void updateCollectedStatus();
        void updateUncollectedStatus();
};