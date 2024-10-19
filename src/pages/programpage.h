#pragma once

#include "basepage.h"
#include "highlighter.h"

#include <QTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class programpage; }
QT_END_NAMESPACE

class programpage : public basepage
{
    Q_OBJECT

    private:
        Ui::programpage* ui;
        Highlighter* highlighter;

    protected:
        ;

    public:
        explicit programpage(QWidget* parent = nullptr);
        ~programpage();

    protected:
        // bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        virtual void resetPage();

        void back();

        void getResult(QNetworkReply* reply);

    signals:
        ;

    private:
        void initalProgramPage();

        void setupEditor();
};