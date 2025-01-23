#include "coursepage.h"
#include "ui_coursepage.h"

#include "markdownhighlighter.h"

int bigChinese_Transfer_Number(const QString& str)
{
    QStringList numerical_value  = {"零","一","二","三","四","五","六","七","八","九"};

    for(int i=0;i<numerical_value.size();i++)
    {
        if(str==numerical_value[i])
            return i;
    }

    return -1;
}

const QString number_Transfer_BigChinese(const double &Fnumber)
{
 
    if(qAbs(Fnumber)<0.01)   //保留2位小数时，近似看作零的情况
        return "零";
 
    //判断正负号
    QString numberSign;//存储符号
    if(Fnumber<0)
        numberSign = "（负数）";
   //将数据的绝对值 转换成字符串，如-58 转成 “58.00”
   QString number = QString::number(qAbs(Fnumber),'f',2);//qAbs绝对值 ，保留两位小数
   QString Left_of_Point;//整数部分
   int length =number.length()-3;//整数部分的长度，（精确度为2，去除小数和小数点）
   if(length>12)
   {
        //qDebug()<<"输入的数值超过范围！"
        return "输入的数值超过范围！";
   }
 
//    QStringList numerical_unit   =   {"","拾","佰","仟","万","拾", "佰", "仟", "亿", "拾", "佰", "仟"};
//    QStringList numerical_value  =   {"零","壹","贰","叁","肆","伍","陆","柒","捌","玖"};

   QStringList numerical_unit   =   {"","十","百","千","万","十", "百", "千", "亿", "十", "百", "千"};
   QStringList numerical_value  =   {"零","一","二","三","四","五","六","七","八","九"};
 
   //数数整数低位多少个连零
   int counter_0=0;
   for(int i =length-1;i>=0;i--)
   {
       if((number[i].toLatin1()-'0' )==0) //从个位开始，向高位走
           counter_0++;
       else
           break;
 
   }
   if(length==1 && counter_0==1) //0.x
       counter_0=0; //不进行过滤
 
   //1400  0.2
 
   for(int i=0,flag=1;i<length-counter_0;i++)
   {
        //5     8       1    2   3
        //伍 拾 捌       壹佰 贰拾 叁
       if((number[i].toLatin1()-'0')==0)
       {
           if((flag!=0 && (length-1-i)%4 != 0) || length ==1) //flag！=0  表示前一个数值 不为0
           Left_of_Point+="零"; //后面不用添加 单位
           if((length-1-i)%4 == 0) //如果0处于分段处，后面需添加单位
           Left_of_Point+=numerical_unit[length-1-i];//添加数值单位
 
           flag =0; //标记
 
       }
       else
       {
        flag =1;
        Left_of_Point+=numerical_value[number[i].toLatin1()-'0']; //'5'-'0'==5
        Left_of_Point+=numerical_unit[length-1-i];//添加数值单位
 
       }
   }
    //QString Right_of_Point;//小数点右侧，小数部分(保留两位)  xxxx.yy
    int totalLength = number.length();
    QString suffix;

    if(number[totalLength-2]=='0'&&number[totalLength-1]=='0')
    {
        QString Bigcn=numberSign+Left_of_Point+suffix;
        return Bigcn;
    }
    else if(number[totalLength-2]!='0'&&number[totalLength-1]=='0')
    {
        QString Bigcn=numberSign+Left_of_Point+"点"+numerical_value[number[totalLength-2].toLatin1()-'0']+suffix;
        return Bigcn;
    }
    else
    {
        QString Bigcn=numberSign+Left_of_Point+"点"+numerical_value[number[totalLength-2].toLatin1()-'0']+numerical_value[number[totalLength-1].toLatin1()-'0']+suffix;
        return Bigcn;
    }
 
 
    return "异常情况";
}

coursepage::coursepage(QWidget *parent)
    : basepage("在线学习",parent)
    , ui(new Ui::coursepage)
    , m_CurrChapter(-1), m_CurrSection(-1)
{
    initalCoursePage();
}

coursepage::~coursepage()
{
}

QString coursepage::getOutLine(QTreeWidget* w)
{
    QTreeWidgetItemIterator it(w);

    jsonFile json;

    QString currChapter;
    QList<QPair<QString,QString>> currSections;
    int countOfChapter=0;
    int countOfSection=0;

    // reg 用于匹配章
    QRegularExpression reg("^\u7b2c[\u4e00-\u9fa5]\u7ae0$");
    QRegularExpressionValidator v(reg,0);

    while(*it)
    {
        // 若列数少于2，则下一列
        if((*it)->columnCount()<2)
            continue;
        
        // 获取章节名称
        QString str=(*it)->text(0);

        int pos=0;
        QValidator::State result=v.validate(str,pos);

        // 判断章节，result 不匹配则是章，否则则是节
        if(result!=QValidator::State::Acceptable)
        {
            // 若为节

            // 将这一节插入数组中
            currSections.append(QPair<QString,QString>(QString::number(countOfSection),(*it)->text(1)));
            countOfSection++;
        }
        else
        {
            // 若为章

            if(!currChapter.isEmpty()&&!currSections.isEmpty())
            {
                currSections.append(QPair<QString,QString>("count",QString::number(countOfSection)));

                json.add(QString::number(countOfChapter),currSections);
                countOfChapter++;
            }

            currChapter=(*it)->text(1);

            // 清空存储节的数组，插入章的名字
            currSections.clear();
            currSections.append(QPair<QString,QString>("value",(*it)->text(1)));

            countOfSection=0;
        }
        
        it++;
    }

    // 将最后一章插入
    if(!currChapter.isEmpty()&&!currSections.isEmpty())
    {
        currSections.append(QPair<QString,QString>("count",QString::number(countOfSection)));

        json.add(QString::number(countOfChapter),currSections);
        countOfChapter++;
    }

    json.add("count",QString::number(countOfChapter));

    return json.toString();
}

QPair<QPair<int, int>, QString> coursepage::getCurrCourse()
{
    QPair<QPair<int,int>,QString> data;
    QPair<int,int> index;
    index.first=m_CurrChapter;
    index.second=m_CurrSection;
    data.first=index;
    data.second=ui->content->toPlainText();

    return data;
}

void coursepage::resetPage()
{
}

// 页面初始化时，向后端请求课程大纲内容
void coursepage::selectedPage()
{
    connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&coursepage::setOutLine);

    QNetworkRequest request;
    request.setUrl(URL_OF_SERVER+"/Course/getOutline");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
    request.setRawHeader("Accept","text/html");

    QJsonObject obj;
    QJsonDocument doc(obj);

    HTTP_MANAGER->post(request,doc.toJson());
}

void coursepage::back()
{
}

// 接收后端响应，设置页面课程大纲内容
void coursepage::setOutLine(QNetworkReply* reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&coursepage::setOutLine);

    QString str=reply->readAll();

    azh::logger()<<"coursepage getOutline:"<<str;

    QTreeWidget* widgetOfOutLine=ui->outline;

    widgetOfOutLine->clear();
    widgetOfOutLine->setHeaderItem(new QTreeWidgetItem(QStringList()<<"章节"<<"名称"));

    QJsonObject obj=jsonFile::toJson(str);
    QString currChapter;
    QList<QPair<QString,QString>> currSections;

    int countOfChapter=obj.value("count").toInt();

    for(int i=0;i<countOfChapter;i++)
    {
        QJsonObject chapter=obj.value(QString::number(i)).toObject();
        
        int countOfSection=chapter.value("count").toInt();
        QString value=chapter.value("value").toString();

        QTreeWidgetItem* item=new QTreeWidgetItem(QStringList()<<"第"+number_Transfer_BigChinese(i+1)+"章"<<value);

        for(int j=0;j<countOfSection;j++)
        {
            value=chapter.value(QString::number(j)).toString();

            QTreeWidgetItem* child=new QTreeWidgetItem(QStringList()<<"第"+number_Transfer_BigChinese(j+1)+"节"<<value);

            item->addChild(child);
        }

        widgetOfOutLine->addTopLevelItem(item);
    }
}

void coursepage::setContent(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&coursepage::setContent);

    QString str=reply->readAll();
    QJsonObject obj=jsonFile::toJson(str);

    ui->content->setAutoTextOptions(QMarkdownTextEdit::BracketRemoval);
    ui->content->setPlainText(obj.value("content").toString());
    ui->content->setReadOnly(true);
}

void coursepage::setEditable(bool status)
{
    ui->content->setReadOnly(!status);
}

void coursepage::initalCoursePage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/course");

    ui->btnOfMark->hide();

    ui->outline->columnCount();

    getOutLine(ui->outline);

    connect(ui->outline,&QTreeWidget::clicked,this,[=]()
    {
        QList<QTreeWidgetItem*> list=ui->outline->selectedItems();
        for(int i=0;i<list.count();i++)
        {
            QTreeWidgetItemIterator it(list[i]);

            QString str=(*it)->data(0,0).value<QString>();

            QString chapter;
            QString section;

            if(str.indexOf("章")==-1)
            {
                QString chapterStr=(*it)->parent()->data(0,0).value<QString>();
                chapter=QString::number(bigChinese_Transfer_Number(chapterStr[1])-1);
                section=QString::number(bigChinese_Transfer_Number(str[1]));
            }
            else
            {
                chapter=QString::number(bigChinese_Transfer_Number(str[1])-1);;
                section="0";
            }

            m_CurrChapter=chapter.toInt();
            m_CurrSection=section.toInt();

            connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&coursepage::setContent);

            QNetworkRequest request;
            request.setUrl(URL_OF_SERVER+"/Course/getCourse");
            request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
            request.setRawHeader("Accept","text/html");

            QJsonObject obj;
            obj.insert("chapter",chapter);
            obj.insert("section",section);
            QJsonDocument doc(obj);

            HTTP_MANAGER->post(request,doc.toJson());
        }
    });
}
