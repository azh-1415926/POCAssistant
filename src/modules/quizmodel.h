#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class quizmodel : public QObject
{
    Q_OBJECT

    private:
        QList<QJsonObject> m_Data;

    public:
        explicit quizmodel(QObject* parent = nullptr);
        virtual ~quizmodel();
        
        inline int count() { return m_Data.count(); }
        QJsonObject get(int i) { return m_Data[i]; }
        bool getCollectedStatus(int i) { return m_Data[i].value("collected").toInt(); }
        QJsonObject getCollectedIndex()
        {
            QJsonObject data;

            QJsonArray list;

            for(int i=0;i<m_Data.size();i++)
            {
                if(m_Data[i].value("collected").toInt()==1)
                    list.append(i);
            }

            data.insert("data",list);

            return data;
        }

        QJsonObject getUncollectedIndex()
        {
            QJsonObject data;

            QJsonArray list;

            for(int i=0;i<m_Data.size();i++)
            {
                if(m_Data[i].value("collected").toInt()==0)
                    list.append(i);
            }

            data.insert("data",list);

            return data;
        }

        void setAnswer(int i,const QString& str) { m_Data[i].insert("answer",str); }
        QStringList getAnswer()
        {
            QStringList list;

            for(auto i : m_Data)
            {
                list<<i.value("answer").toString();
            }

            return list;
        }

    protected:
        ;

    public slots:
        void load(QJsonObject json);
        void setCollected(int i,bool isCollected);

    signals:
        void dataChanged();

    private:
        ;
};