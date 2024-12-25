#pragma once

#include <QString>
#include <QPixmap>

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