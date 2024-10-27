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

    protected:
        ;

    public slots:
        void load(QJsonObject json);

    signals:
        void dataChanged();

    private:
        ;
};