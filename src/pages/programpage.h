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

        void getExperiment(QNetworkReply* reply);
        void getResult(QNetworkReply* reply);
        void getSubmitState(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalProgramPage();

        void setupEditor();
};