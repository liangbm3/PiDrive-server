#include "tcpserver.h"
#include "./ui_tcpserver.h"
#include "mytcpserver.h"
#include <QFile>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

TcpServer::TcpServer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    //指定json资源文件路径
    QFile file(":/config.json");
    //尝试以只读方式打开
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning()<<"无法打开配置文件!";
        return;
    }

    //读取整个json文件内容
    QByteArray json_data =file.readAll();
    file.close();//关闭文件

    //解析json数据
    QJsonParseError json_error;//用于存储解析过程中的错误信息
    QJsonDocument json_doc=QJsonDocument::fromJson(json_data,&json_error);

    //检查json是否成功解析
    if (json_error.error!=QJsonParseError::NoError)
    {
        qWarning()<<"配置文件解析错误"<<json_error.errorString();
        return;
    }

    //确保json根元素是对象
    if(!json_doc.isObject())
    {
        qWarning()<<"配置文件格式错误！";
        return;
    }

    //获取json根对象
    QJsonObject json_object=json_doc.object();

    //读取ip字段，字符串类型
    if(json_object.contains("ip") && json_object["ip"].isString())
    {
        m_strIP=json_object["ip"].toString();
        qDebug()<<"ip:"<<m_strIP.toStdString();
    }

    //读取port字段，数字类型
    if(json_object.contains("port") && json_object["port"].isDouble())
    {
        m_usPort=json_object["port"].toInt();
        qDebug()<<"port:"<<m_usPort;
    }


}
