#include "mainpage.h"
#include "ui_mainpage.h"

mainpage::mainpage(QWidget *parent)
    : basepage("主页",parent)
    , ui(new Ui::mainpage)
{
    initalMainPage();
}

mainpage::~mainpage()
{
}

void mainpage::resetPage()
{
}

void mainpage::back()
{
}

// 所在图标的标签被点击，发送 selectPage 信号
void mainpage::selectLabel(const clicklabel* label)
{
    // course(management)
    if(label==ui->course)
    {
        // 若为管理员，则跳转至在线管理
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(4);
            return;
        }

        emit selectPage(1);
    }
    // program
    else if(label==ui->program)
    {
        // 若为管理员，则跳转至在线统计
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(5);
            return;
        }

        emit selectPage(2);
    }
    else if(label==ui->quiz)
    {
        // 若为管理员，则跳转至在线更新
        if(userRole::getInstance().get()==(int)(UserRole::ADMIN))
        {
            emit selectPage(6);
            return;
        }

        emit selectPage(3);
    }
    else
    {
        ;
    }
}

// 切换不同的用户形态
void mainpage::switchTo(int role)
{
    UserRole userRole=(UserRole)role;

    QString textOfFirst;
    QString textOfSecond;
    QString textOfThird;
    QList<QPixmap> icons=
    {
        QPixmap(),
        QPixmap(),
        QPixmap()
    };

    switch (userRole)
    {
    case UserRole::STUDENT:
        ui->course->setText("在线课程");
        ui->program->setText("在线实验");
        ui->quiz->setText("在线测试");
        icons[0].load(":/img/main/course");
        icons[1].load(":/img/main/code");
        icons[2].load(":/img/main/quiz");
        
        textOfFirst="<html><head/><body><p>提供编译原理的学习路线，涵盖编译原理的各个方面，如词法分析、语法分析、语义分析等</p></body></html>";
        textOfSecond="<html><head/><body><p>提供在线代码编辑器，支持C语言，用户可以在线编写代码并进行编译，查看编译结果</p></body></html>";
        textOfThird="<html><head/><body><p>提供编译原理相关的在线测试题目，包括选择题、填空题、编程题等，帮助用户检验学习成果</p></body></html>";
        break;
    
    case UserRole::TEACHER:
        ui->course->setText("在线课程");
        ui->program->setText("在线实验");
        ui->quiz->setText("在线测试");
        icons[0].load(":/img/main/course");
        icons[1].load(":/img/main/code");
        icons[2].load(":/img/main/quiz");

        textOfFirst="<html><head/><body><p>提供编译原理的学习路线，涵盖编译原理的各个方面，如词法分析、语法分析、语义分析等</p></body></html>";
        textOfSecond="<html><head/><body><p>可在线批改学生代码功能，对所属班级中学生的代码查阅并编译运行，查看编译结果</p></body></html>";
        textOfThird="<html><head/><body><p>可在线批改学生课后习题，仅需批改简答题，自动批改选择填空题，编程题位于在线实验中</p></body></html>";
        break;
    
    case UserRole::ADMIN:
        ui->course->setText("在线管理");
        ui->program->setText("在线统计");
        ui->quiz->setText("在线更新");
        icons[0].load(":/img/main/management");
        icons[1].load(":/img/main/statement");
        icons[2].load(":/img/main/update");
        
        textOfFirst="<html><head/><body><p>提供在线管理功能，管理员管理学生、教师账号，创建班级，分配教师、学生</p></body></html>";
        textOfSecond="<html><head/><body><p>提供在线统计功能，管理员可统计所有用户的学习情况、实验完成情况、答题情况</p></body></html>";
        textOfThird="<html><head/><body><p>提供在线管理功能，管理员负责对编译原理课程大纲进行更新，对题目进行更新</p></body></html>";
        break;
    
    default:
        break;
    }

    ui->iconOfCourse->setPixmap(icons[0]);
    ui->iconOfProgram->setPixmap(icons[1]);
    ui->iconOfQuiz->setPixmap(icons[2]);

    ui->textOfCourse->setText(textOfFirst);
    ui->textOfProgram->setText(textOfSecond);
    ui->textOfQuiz->setText(textOfThird);
}

void mainpage::initalMainPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/home");

    // 绑定图标标签、按钮到 selectLabel 槽函数
    connect(ui->course,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->program,&clicklabel::clicked,this,&mainpage::selectLabel);
    connect(ui->quiz,&clicklabel::clicked,this,&mainpage::selectLabel);

    connect(ui->btnOfCourse,&QPushButton::clicked,this,[=](){ selectLabel(ui->course); });
    connect(ui->btnOfProgram,&QPushButton::clicked,this,[=](){ selectLabel(ui->program); });
    connect(ui->btnOfQuiz,&QPushButton::clicked,this,[=](){ selectLabel(ui->quiz); });
}
