#include "quizview.h"

#include "collectbutton.h"
#include "clickoptions.h"
#include "highlighter.h"

#include <QGridLayout>
#include <QVBoxLayout>

quizview::quizview(QWidget *parent)
    : QWidget(parent)
    , optionOfSelectQuiz(new clickoptions)
    , optionOfJudgeQuiz(new clickoptions(2))
    , fillQuiz(new QTextEdit)
    , codeQuiz(new QTextEdit)
    , forwardBtn(new QPushButton), nextBtn(new QPushButton), collectBtn(new collectbutton)
    , textOfTag(new QLabel), textOfProcess(new QLabel), textOfQuiz(new QLabel)
{
    initalCodeQuiz();
    initalQuestion();
}

quizview::~quizview()
{
    delete optionOfSelectQuiz;
    delete optionOfJudgeQuiz;
    delete fillQuiz;
    delete codeQuiz;
    delete forwardBtn;
    delete nextBtn;
    delete collectBtn;
    delete textOfTag;
    delete textOfQuiz;
}

/* 设置当前组件的标题 */
void quizview::setTitle(const QString& title)
{
    this->textOfTag->setText(title);
}

/* 设置当前问题的下标 */
void quizview::setIndex(int i)
{
    QString sum="0";
    QStringList list=this->textOfProcess->text().split("/");
    if(!list.isEmpty()&&list.count()>1)
        sum=list[1];

    textOfProcess->setText(QString::number(i+1)+"/"+sum);
}

/* 设置当前问题的总数 */
void quizview::setSum(int n)
{
    QString index="0";
    QStringList list=this->textOfProcess->text().split("/");
    if(!list.isEmpty())
        index=list[0];

    textOfProcess->setText(index+"/"+QString::number(n));
}

/* 设置当前问题下标对应标签的文本 */
void quizview::setTextOfIndex(const QString &str)
{
    QString sum="0";
    QStringList list=this->textOfProcess->text().split("/");
    if(!list.isEmpty())
        sum=list[1];

    this->textOfProcess->setText(str+"/"+sum);
}

/* 设置当前问题总数对应标签的文本 */
void quizview::setTextOfSum(const QString &str)
{
    QString index="0";
    QStringList list=this->textOfProcess->text().split("/");
    if(!list.isEmpty())
        index=list[0];
        
    this->textOfProcess->setText(index+"/"+str);
}

/* 设置当前问题收藏的状态，true 为设置为被收藏 */
void quizview::setCollect(bool status)
{
    if(status)
        this->collectBtn->setCollectNoSignal();
    else
        this->collectBtn->setUncollectNoSignal();
}

void quizview::setQuizType(int type)
{
    QString quizType;

    switch (type)
    {
    case 0:
        quizType="选择题";
        break;

    case 1:
        quizType="判断题";
        break;

    case 2:
        quizType="填空题";
        break;

    case 3:
        quizType="编程题";
        break;

    default:
        break;
    }

    centerOfQuiz->setCurrentIndex(type);
    textOfTag->setText(quizType);
}

void quizview::hideCollection(bool b)
{
    if(b)
        collectBtn->hide();
    else
        collectBtn->show();
}

void quizview::hideHover(bool b)
{
    optionOfSelectQuiz->hideHoverBox(b);
    optionOfJudgeQuiz->hideHoverBox(b);
}

void quizview::setReadOnly(bool b)
{
    optionOfSelectQuiz->setReadOnly(b);
    optionOfJudgeQuiz->setReadOnly(b);
}

void quizview::updateQuiz(const QJsonObject &quiz)
{
    int type=quiz.value("type").toInt();

    setQuizType(type);

    textOfQuiz->setText(quiz.value("content").toString());

    switch (type)
    {
    case 0:
        emit updateSelectOption(0,quiz.value("A").toString());
        emit updateSelectOption(1,quiz.value("B").toString());
        emit updateSelectOption(2,quiz.value("C").toString());
        emit updateSelectOption(3,quiz.value("D").toString());
        break;

    case 1:

        emit updateJudgeOption(0,"正确");
        emit updateJudgeOption(1,"错误");
        break;

    case 2:
        /* code */
        break;

    case 3:
        /* code */
        break;
    
    default:
        break;
    }
}

void quizview::setOption(int i)
{
    if(i<0||i>3)
        return;

    optionOfSelectQuiz->setOption(i);

    if(i>1)
        return;
        
    optionOfJudgeQuiz->setOption(i);
}

void quizview::setAnswerIndex(int i)
{
    optionOfSelectQuiz->setAnswer(i);
    optionOfJudgeQuiz->setAnswer(i);
}

void quizview::reset()
{
    setIndex(-1);
    setSum(0);

    centerOfQuiz->setCurrentIndex(0);
    textOfTag->setText("");
    textOfQuiz->setText("暂无题目");
    emit updateSelectOption(0,"无内容");
    emit updateSelectOption(1,"无内容");
    emit updateSelectOption(2,"无内容");
    emit updateSelectOption(3,"无内容");
}

/* 初始化布局、按钮、文本 */
void quizview::initalQuestion()
{
    /* 主体采用垂直布局 */
    QVBoxLayout* layout=new QVBoxLayout(this);
    QGridLayout* layoutOfSwitch=new QGridLayout;
    QHBoxLayout* layoutOfStatus=new QHBoxLayout;

    centerOfQuiz=new QStackedWidget;

    layoutOfStatus->addWidget(textOfProcess);
    layoutOfStatus->addWidget(textOfTag);

    /* 切换问题的左右按钮的文本分别设置为 "←"、"→"，设置收藏按钮为取消收藏状态 */
    forwardBtn->setText("←");
    nextBtn->setText("→");
    collectBtn->setUncollectNoSignal();

    /* 将所有组件添加到布局中 */
    layoutOfSwitch->addWidget(forwardBtn,0,0);
    layoutOfSwitch->addWidget(collectBtn,0,1);
    layoutOfSwitch->addWidget(nextBtn,0,2);

    layout->addLayout(layoutOfStatus);
    centerOfQuiz->addWidget(optionOfSelectQuiz);
    centerOfQuiz->addWidget(optionOfJudgeQuiz);
    centerOfQuiz->addWidget(fillQuiz);
    centerOfQuiz->addWidget(codeQuiz);

    // setQuizType(2);

    // textOfQuiz->setText("这是题目的内容，这是一道编程题");

    layout->addWidget(textOfQuiz);
    layout->addWidget(centerOfQuiz);
    layout->addLayout(layoutOfSwitch);

    /* 向前按钮点击事件 */
    connect(forwardBtn,&QPushButton::clicked,this,[=]() { emit prevQuiz(); });
    /* 向后按钮点击事件 */
    connect(nextBtn,&QPushButton::clicked,this,[=]() { emit nextQuiz(); });
    /* 当收藏按钮发送 collected、uncollected 信号时，发送 collectQuestion、uncollectQuestion 信号，当做转发 */
    connect(collectBtn,&collectbutton::collected,this,[=]() { emit collectQuestion(); });
    connect(collectBtn,&collectbutton::uncollected,this,[=]() { emit uncollectQuestion(); });

    connect(this,&quizview::updateSelectOption,optionOfSelectQuiz,&clickoptions::setTextOfOption);
    connect(this,&quizview::updateJudgeOption,optionOfJudgeQuiz,&clickoptions::setTextOfOption);

    connect(optionOfSelectQuiz,&clickoptions::selectOption,this,[=](int i)
    {
        if(i<0||i>3)
            return;

        QStringList list={"A","B","C","D"};

        emit setAnswer(list[i]);
    });

    connect(optionOfJudgeQuiz,&clickoptions::selectOption,this,[=](int i)
    {
        if(i<0||i>1)
            return;

        QStringList list={"对","错"};

        emit setAnswer(list[i]);
    });

    connect(fillQuiz,&QTextEdit::textChanged,this,[=]()
    {
        emit setAnswer(fillQuiz->toPlainText());
    });

    connect(codeQuiz,&QTextEdit::textChanged,this,[=]()
    {
        emit setAnswer(codeQuiz->toPlainText());
    });
}

void quizview::initalCodeQuiz()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    codeQuiz->setFont(font);

    Highlighter* highlighter = new Highlighter(codeQuiz->document());
}
