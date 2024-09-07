#include "programpage.h"
#include "ui_programpage.h"

programpage::programpage(basepage *parent)
    : basepage(parent)
    , ui(new Ui::programpage)
{
    initalProgramPage();
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

    setStatus("课程实践","");
}