#pragma once

#include <QObject>
#include <QJsonObject>

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