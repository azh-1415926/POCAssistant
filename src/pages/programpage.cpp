#include "programpage.h"
#include "ui_programpage.h"

#include <QFile>

programpage::programpage(QWidget *parent)
    : basepage("在线实验",parent)
    , ui(new Ui::programpage)
{
    initalProgramPage();
    
    setupEditor();
}

programpage::~programpage()
{
}

void programpage::resetPage()
{
}

void programpage::back()
{
}

void programpage::initalProgramPage()
{
    ui->setupUi(this);
}

void programpage::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    ui->CodeEdit->setFont(font);

    highlighter = new Highlighter(ui->CodeEdit->document());

    QFile file(":/sample-code/test.cpp");
    if (file.open(QFile::ReadOnly | QFile::Text))
        ui->CodeEdit->setPlainText(file.readAll());
}