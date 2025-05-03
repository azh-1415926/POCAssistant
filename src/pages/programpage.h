#pragma once

#include "basepage.h"
#include "highlighter.h"

#include <QTextEdit>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class programpage; }
QT_END_NAMESPACE

// 在线实验页面，建议对普通用户显示，对管理员隐藏
class programpage : public basepage
{
    Q_OBJECT

    private:
        Ui::programpage* ui;

        // 高亮显示渲染
        Highlighter* highlighter;
        // 学生未完成的实验信息数据
        QJsonObject m_UnFinishedExperimentInfo;
        // 教师所管理的班级列表
        QList<QPair<QString,QString>> m_ClassInfo;

    protected:
        ;

    public:
        explicit programpage(QWidget* parent = nullptr);
        ~programpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();
        virtual void selectedPage();

        void back();

        // 获取当前实验清单
        void getUnfinishedExperiment(QNetworkReply* reply);
        // 获取编译结果
        void getCompileOutput(QNetworkReply* reply);
        // 获取提交结果
        void getSubmitState(QNetworkReply* reply);
        // 获取班级列表
        void getClassInfo(QNetworkReply* reply);
        // 获取发布实验的结果
        void getReleaseState(QNetworkReply* reply);
        // 获取当前用户全部实验
        void getAllExperiment(QNetworkReply* reply);
        // 获取当前教师管理班级的全部实验
        void getExperimentByTeacher(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalProgramPage();

        void setupEditor();

        void requestUnfinishedExperiment();
};