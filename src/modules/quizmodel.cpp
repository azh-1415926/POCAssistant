#include "quizmodel.h"

quizmodel::quizmodel(QObject *parent)
{
}

quizmodel::~quizmodel()
{
}

void quizmodel::setCollected(int i, bool isCollected)
{
    if(i<0||i>=m_Data.size())
        return;
    
    m_Data[i].value("collected")=(int)isCollected;
}

void quizmodel::load(QJsonObject json)
{
    m_Data.clear();

    int count=json.value("count").toInt();

    for(int i=0;i<count;i++)
    {
        m_Data.push_back(json.value(QString::number(i)).toObject());
    }

    emit dataChanged();
}