#include "quizview.h"

#include "collectbutton.h"
#include "clickoptions.h"

#include <QGridLayout>
#include <QVBoxLayout>

quizview::quizview(QWidget *parent)
    : QWidget(parent)
    , options(new clickoptions)
    , indexOfQuestion(0), sumOfQuestion(0)
    , forwardBtn(new QPushButton), nextBtn(new QPushButton), collectBtn(new collectbutton)
    , textOfTag(new QLabel), textOfProcess(new QLabel)
{
    initalQuestion();
}

quizview::~quizview()
{
    delete options;
    delete forwardBtn;
    delete nextBtn;
    delete collectBtn;
    delete textOfTag;
}

/* 设置当前组件的标题 */
void quizview::setTitle(const QString& title)
{
    this->textOfTag->setText(title);
}

/* 设置当前问题的下标 */
void quizview::setIndex(int i)
{
    this->indexOfQuestion=i;
    if(this->indexOfQuestion<this->sumOfQuestion)
        this->textOfProcess->setText(QString::number(this->indexOfQuestion+1)+"/"+QString::number(sumOfQuestion));
    else
        this->textOfProcess->setText(QString::number(this->indexOfQuestion)+"/"+QString::number(sumOfQuestion));
}

/* 设置当前问题的总数 */
void quizview::setSum(int n)
{
    this->sumOfQuestion=n;
    this->textOfProcess->setText(QString::number(this->indexOfQuestion)+"/"+QString::number(sumOfQuestion));
}

/* 设置当前问题下标对应标签的文本 */
void quizview::setTextOfIndex(const QString &str)
{
    this->textOfProcess->setText(str+"/"+QString::number(sumOfQuestion));
}

/* 设置当前问题总数对应标签的文本 */
void quizview::setTextOfSum(const QString &str)
{
    this->textOfProcess->setText(QString::number(this->indexOfQuestion)+"/"+str);
}

/* 设置当前问题收藏的状态，true 为设置为被收藏 */
void quizview::setCollect(bool status)
{
    if(status)
        this->collectBtn->setCollectNoSignal();
    else
        this->collectBtn->setUncollectNoSignal();
}

/* 初始化布局、按钮、文本 */
void quizview::initalQuestion()
{
    /* 主体采用垂直布局 */
    QVBoxLayout* layout=new QVBoxLayout(this);
    QGridLayout* layoutOfSwitch=new QGridLayout;
    QHBoxLayout* layoutOfStatus=new QHBoxLayout;

    textOfProcess->setText("1/15");
    textOfTag->setText("选择题");

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
    layout->addWidget(options);
    layout->addLayout(layoutOfSwitch);

    /* 向前按钮点击事件 */
    connect(forwardBtn,&QPushButton::clicked,this,[=]()
    {
        /* 若下标小于等于 0，即左边已经没有问题了，直接返回 */
        if(this->indexOfQuestion<=0)
            return;
        /* 若下标合法，发送 lastIndex 信号，传入未切换前的下标 */
        emit lastIndex(this->indexOfQuestion--);
        /* 默认设置下标文本为当前下标+1，发送 changeQuestion 信号，传入切换后的下标 */
        setTextOfIndex(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    /* 向后按钮点击事件 */
    connect(nextBtn,&QPushButton::clicked,this,[=]()
    {
        /* 若下标大于等于问题的总数，即右边已经没有问题了，直接返回 */
        if(this->indexOfQuestion>=this->sumOfQuestion-1)
            return;
        /* 若下标合法，发送 lastIndex 信号，传入未切换前的下标 */
        emit lastIndex(this->indexOfQuestion++);
        /* 默认设置下标文本为当前下标+1，发送 changeQuestion 信号，传入切换后的下标 */
        setTextOfIndex(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    /* 当收藏按钮发送 collected、uncollected 信号时，发送 collectQuestion、uncollectQuestion 信号，当做转发 */
    connect(collectBtn,&collectbutton::collected,this,[=]()
    {
        emit collectQuestion();
    });
    connect(collectBtn,&collectbutton::uncollected,this,[=]()
    {
        emit uncollectQuestion();
    });
}