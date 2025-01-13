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

// 单例模式，设置初始值
#define SINGLETONE_WITH_VALUE(Name,Type,Value) \
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
        explicit Name(QObject* parent=nullptr): QObject(parent) \
        { \
            m_Instance=Value; \
        } \
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

// 取出指定单例类中的值
#define SINGLETONE_VALUE(Name) \
    Name::getInstance().get()