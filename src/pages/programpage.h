#pragma once

#include "basepage.h"
#include "highlighter.h"

#include <QTextEdit>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class programpage; }
QT_END_NAMESPACE

class programpage : public basepage
{
    Q_OBJECT

    private:
        Ui::programpage* ui;
        Highlighter* highlighter;
        QJsonObject m_ExperimentInfo;

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
        void getExperiment(QNetworkReply* reply);
        // 获取编译结果
        void getResult(QNetworkReply* reply);
        // 获取提交结果
        void getSubmitState(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalProgramPage();

        void setupEditor();
};