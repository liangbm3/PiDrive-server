#include "operatedb.h"
#include <QMessageBox>

OperateDB::OperateDB(QObject *parent)
    : QObject{parent}
{
    m_db=QSqlDatabase::addDatabase("QSQLITE");
}

OperateDB &OperateDB::getInstance()
{
    static OperateDB instance;
    return instance;
}

void OperateDB::init()
{
    m_db.setHostName("localhost");
    m_db.setDatabaseName("D:\\wangpan\\tcp_server\\cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select * from usrinfo");
        while(query.next())
        {
            QString data = QString("%1, %2, %3, %4").arg(query.value(0).toString()).arg(query.value(1).toString())
            .arg(query.value(2).toString()).arg(query.value(3).toString());
            qDebug()<<data;
        }
    }
    else
    {
        QMessageBox::critical(NULL,"查询数据库","查询数据库失败");
    }
}

OperateDB::~OperateDB()
{
    m_db.close();
}

bool OperateDB::handleRegist(const char *name, const char *pwd)
{
    //考虑极端情况
    if(NULL==name ||NULL==pwd)
    {
        return false;
    }
    else
    {
        QString strQuery=QString("insert into usrinfo(name,pwd) values('%1','%2')").arg(name).arg(pwd);
        QSqlQuery query;
        qDebug()<<strQuery;
        return query.exec(strQuery);
    }
}

bool OperateDB::handleLogin(const char *name, const char *pwd)
{
    if(NULL==name||NULL==pwd)
    {
        return false;
    }
    QString strQuery=QString("select * from usrinfo "
                               "where name ='%1' and pwd ='%2' and online=0").arg(name).arg(pwd);
    QSqlQuery query;
    qDebug()<<strQuery;
    query.exec(strQuery);
    if(query.next())//每次调用next都会读取一条数据，并将结果放入query中，返回值为true，无数据则返回false
    {

        strQuery=QString("update usrinfo set online=1 "
                           "where name ='%1' and pwd ='%2'").arg(name).arg(pwd);
        qDebug()<<strQuery;
        return query.exec(strQuery);
    }
    else
    {
        qDebug()<<"登录失败";
        return false;
    }
}

bool OperateDB::handleOffline(const char *name)
{
    qDebug()<<"enter handleOffline";
    if(NULL==name)
    {
        qDebug()<<"name is NULL";
        return false;
    }
    QString strQuery =QString("update usrinfo set online=0 "
                           "where name='%1'").arg(name);
    qDebug()<<strQuery;
    QSqlQuery query(strQuery);
    bool result=query.exec(strQuery);
    if(result)
    {
        qDebug()<<"sucess";
    }
    else
    {
        qDebug()<<"fail";
    }
    return result;
}

