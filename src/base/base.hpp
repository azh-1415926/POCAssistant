#pragma once

#include <QString>
#include <QPixmap>

// 单例模式，存取值变量
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

// 单例模式，存储动态对象
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

// 页面的基本信息
struct StatusOfPage
{
    QString currPage;
    QPixmap currIcon;
    QString currInfo;
};

// 用户类型
enum class UserRole : int
{
    STUDENT=0,
    TEACHER=1,
    ADMIN=2
};