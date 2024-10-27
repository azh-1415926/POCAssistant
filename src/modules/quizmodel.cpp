#include "quizmodel.h"

quizmodel::quizmodel(QObject *parent)
{
}

quizmodel::~quizmodel()
{
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