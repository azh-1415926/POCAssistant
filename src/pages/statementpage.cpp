#include "statementpage.h"
#include "ui_statementpage.h"

statementpage::statementpage(QWidget *parent)
    : basepage("在线统计",parent)
    , ui(new Ui::statementpage)
{
    initalStatementPage();
}

statementpage::~statementpage()
{
}

void statementpage::resetPage()
{
}

void statementpage::back()
{
}

void statementpage::getStatement(QNetworkReply *reply)
{
    disconnect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&statementpage::getStatement);

    QString str=reply->readAll();
    
    azh::logger()<<"statement getstatement:"<<str;

    jsonFile json;
    json.fromJson(str);

    QString result=json.value("result").toString();

    if(!result.isEmpty())
    {

        if(result=="true")
        {
            int row=json.value("row").toInt();
            int col=json.value("col").toInt();
            ui->tableOfStatement->clear();
            ui->tableOfStatement->setRowCount(row-1);
            ui->tableOfStatement->setColumnCount(col);

            QStringList title;

            for(int i=0;i<col;i++)
            {
                QJsonObject recordOfTitle=json.value("0").toObject();
                title<<recordOfTitle.value(QString::number(i)).toString();
            }

            ui->tableOfStatement->setHorizontalHeaderLabels(title);

            for(int i=1;i<row;i++)
            {
                QJsonObject record=json.value(QString::number(i)).toObject();
                
                for(int j=0;j<col;j++)
                {
                    ui->tableOfStatement->setItem(i-1,j,new QTableWidgetItem(record.value(QString::number(j)).toString()));
                }
            }
        }
    }
    // 若响应为空，则说明请求超时，直接退出登陆
    else
    {
        emit logoff();
    }
}

void statementpage::initalStatementPage()
{
    ui->setupUi(this);

    m_Status.currIcon.load(":/img/main/statement");

    connect(ui->optionOfStatement,&QComboBox::currentIndexChanged,this,[=](int i)
    {
        connect(HTTP_MANAGER, &QNetworkAccessManager::finished,this,&statementpage::getStatement);
        QNetworkRequest request;
        request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWe");
        request.setRawHeader("Accept","text/html");
    
        QJsonObject data;
    
        // token required
        data.insert("token",tokenOfAdmin::getInstance().get());
    
        QJsonDocument doc(data);
    
        if(i==0)
        {
            request.setUrl(URL_OF_SERVER+"/Manager/statUser");
        }
        else if(i==1)
        {
            request.setUrl(URL_OF_SERVER+"/Manager/statClass");
        }
        else if(i==2)
        {
            request.setUrl(URL_OF_SERVER+"/Manager/statQuiz");
        }

        HTTP_MANAGER->post(request,doc.toJson());
    });

    ui->optionOfStatement->addItems(QStringList()<<"用户统计"<<"班级统计"<<"题目统计");
}
