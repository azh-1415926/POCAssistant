#ifndef CLICK_OPTIONS_H
#define CLICK_OPTIONS_H

#include <QGroupBox>
#include <QEvent>
#include <QRadioButton>

class clicklabel;

class clickoptions : public QGroupBox
{
    Q_OBJECT

    private:
        int numOfOptions;
        int answerOfOptions;
        int hoverOption;
        int checkedOption;
        QList<QRadioButton*> buttons;
        QList<clicklabel*> labels;
        QRect* hoverBox;
        QRect* checkedBox;
        QRect* correctBox;
        QRect* incorrectBox;

        bool flagOfHideHoverBox;
        bool flagOfReadOnly;

    public:
        explicit clickoptions(QWidget* parent = nullptr);
        clickoptions(int nOfOption,QWidget* parent = nullptr);
        virtual ~clickoptions();
        /* 返回答案选项的下标 */
        inline int getAnswer() const { return this->answerOfOptions; };

    protected:
        bool eventFilter(QObject *obj, QEvent *e) override;

    public slots:
        void setTextOfOption(int i,const QString& text);
        void setAnswer(int i);
        void displayHover(const clicklabel* label);
        void displayAnswer(bool state);
        void resetOption();

        void hideHoverBox(bool status) { flagOfHideHoverBox=status; }
        void setReadOnly(bool status) { flagOfReadOnly=status; hideHoverBox(status);  }
        void setOption(int i);

    signals:
        void selectOption(int);

    private:
        void initalOptions();
        void initalEvent();
        void paintBox(QWidget* widget,QRect* box);
        void freeBoxes();
        QRect* setOptionOfBox(int i,QRect* rect=nullptr);
};

#endif