#ifndef SWITCH_QUESTION_H
#define SWITCH_QUESTION_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class collectbutton;
class clickoptions;

class quizview : public QWidget
{
    Q_OBJECT

    private:
        clickoptions* options;

        int indexOfQuestion;
        int sumOfQuestion;
        QPushButton* forwardBtn;
        QPushButton* nextBtn;
        collectbutton* collectBtn;
        QLabel* textOfTag;
        QLabel* textOfProcess;

    public:
        explicit quizview(QWidget* parent = nullptr);
        virtual ~quizview();
        
        /* 返回当前组件的标题 */
        inline QString title() const { return this->textOfTag->text(); }
        /* 返回当前问题的下标 */
        inline int index() const { return this->indexOfQuestion; }
        /* 返回当前问题下标对应标签上的字符串文本 */
        inline QString stringOfIndex() const { return QString::number(indexOfQuestion); }
        /* 返回当前问题的总数 */
        inline int count() const { return this->sumOfQuestion; }

    protected:
        ;

    public slots:
        void setTitle(const QString& title);
        void setIndex(int i);
        void setSum(int n);
        void setTextOfIndex(const QString& str);
        void setTextOfSum(const QString& str);
        void setCollect(bool status);

    signals:
        void collectQuestion();
        void uncollectQuestion();
        void changeQuestion(int i);
        void lastIndex(int i);

    private:
        void initalQuestion();
};

#endif