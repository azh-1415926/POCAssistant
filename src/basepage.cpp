#include "basepage.h"

basepage::basepage(QWidget *parent)
    : QWidget(parent)
{
    initalBasePage();
}

basepage::~basepage()
{
}

void basepage::select()
{
    emit refreshStatus(m_Status);
}

void basepage::setStatus(const QString &currPage, const QString &currIcon, const QString &currInfo)
{
    m_Status.currPage=currPage;
    m_Status.currIcon.load(currIcon);
    m_Status.currInfo=currInfo;
}

void basepage::resetPage()
{
}

void basepage::setStatus(const StatusOfPage &status)
{
    m_Status=status;
}

void basepage::back()
{
}

void basepage::initalBasePage()
{
    
}
