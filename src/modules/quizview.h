#pragma once

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QTextEdit>
#include <QJsonObject>

class collectbutton;
class clickoptions;

class quizview : public QWidget
{
    Q_OBJECT

    private:
        QStackedWidget* centerOfQuiz;
        clickoptions* optionOfSelectQuiz;
        clickoptions* optionOfJudgeQuiz;
        QTextEdit* fillQuiz;
        QTextEdit* codeQuiz;

        QPushButton* forwardBtn;
        QPushButton* nextBtn;
        collectbutton* collectBtn;
        QLabel* textOfTag;
        QLabel* textOfProcess;
        QLabel* textOfQuiz;

    public:
        explicit quizview(QWidget* parent = nullptr);
        virtual ~quizview();

    protected:
        ;

    public slots:
        void setTitle(const QString& title);
        void setIndex(int i);
        void setSum(int n);
        void setTextOfIndex(const QString& str);
        void setTextOfSum(const QString& str);
        void setCollect(bool status);

        void setQuizType(int type);
        void hideCollection(bool b);
        void hideHover(bool b);
        void setReadOnly(bool b);
        void updateQuiz(const QJsonObject& quiz);

        void setOption(int i);
        void setAnswerIndex(int i);

        void reset();

    signals:
        void collectQuestion();
        void uncollectQuestion();
        void nextQuiz();
        void prevQuiz();

        void updateSelectOption(int i,const QString& context);
        void updateJudgeOption(int i,const QString& context);

        void setAnswer(const QString& str);

    private:
        void initalQuestion();
        void initalCodeQuiz();
};