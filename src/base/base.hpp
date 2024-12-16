#pragma once

#include <QString>
#include <QPixmap>

#include <QScopedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkCookieJar>

#define SINGLETONE(Name,Type) \
class Name : QObject \
{ \
    public: \
        static Name& getInstance() \
        { \
            static Name instance; \
            return instance; \
        } \
        Type get() {  return m_Instance; } \
        void set(Type data) { m_Instance=data; } \
    private: \
        explicit Name(QObject* parent=nullptr): QObject(parent) { } \
        Type m_Instance; \
};

#define SINGLETONE_P(Name,Type) \
class Name : QObject \
{ \
    public: \
        static Name& getInstance() \
        { \
            static Name instance; \
            return instance; \
        } \
        Type* get() {  return m_Instance; } \
    private: \
        explicit Name(QObject* parent=nullptr): QObject(parent), m_Instance(new Type(this)) { } \
        Type* m_Instance; \
};

struct StatusOfPage
{
    QString currPage;
    QPixmap currIcon;
    QString currInfo;
};

enum class UserRole : int
{
    STUDENT=0,
    TEACHER=1,
    ADMIN=2
};

// template<class T>
// class singletone : QObject
// {
//     public:
//         static singletone& getInstance()
//         {
//             static singletone instance;
//             return instance;
//         }
//         T get() { return m_Instance; }

//         void set(T data) { m_Instance=data; }

//     protected:
//         explicit singletone(QObject* parent=nullptr): QObject(parent) { }

//         T m_Instance;
// };

// class httpManager : QObject
// {
//     public:
//         static httpManager& getInstance()
//         {
//             static httpManager instance;
//             return instance;
//         }
//         QNetworkAccessManager* getManger()
//         {
//             if(m_Manger==nullptr)
//                 m_Manger=new QNetworkAccessManager(this);

//             return m_Manger;
//         }

//     private:
//         explicit httpManager(QObject* parent=nullptr): QObject(parent), m_Manger(nullptr) { }

//         QNetworkAccessManager* m_Manger;
// };
SINGLETONE_P(httpManager,QNetworkAccessManager)
SINGLETONE(userId,QString)
SINGLETONE(userRole,int)