#ifndef COLLOECT_BUTTON_H
#define COLLOECT_BUTTON_H

#include <QPushButton>

class collectbutton : public QPushButton
{
    Q_OBJECT

    private:
        QString textOfCollect;
        QString textOfUncollect;

    public:
        explicit collectbutton(QWidget* parent = nullptr,const QString& collect="⭐",const QString& uncollect="☆");
        virtual ~collectbutton();
        /* 返回是否被收藏，被收藏返回 true */
        inline bool isCollect() const { return this->text()==textOfCollect; };

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void setCollect();
        void setUncollect();
        void setCollectNoSignal();
        void setUncollectNoSignal();

    signals:
        void collected();
        void uncollected();

    private:
        #ifdef __ANDROID__
            bool touchBeginEventProcess(QEvent *e);
        #else
            bool mousePressEventProcess(QEvent* e);
        #endif
};

#endif