#include "mytcpserver.h"

MyTcpServer::MyTcpServer(QObject *parent)
    : QTcpServer{parent}
{}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug()<<"new client connected";
    MyTcpSocket *pTcpSocket =new MyTcpSocket;
    pTcpSocket->setSocketDescriptor(socketDescriptor);
    m_tcpSocketList.append(pTcpSocket);
    connect(pTcpSocket,SIGNAL(offline(MyTcpSocket*)),this,SLOT(deleteSocket(MyTcpSocket*)));
}

void MyTcpServer::deleteSocket(MyTcpSocket *mysocket)
{
    QList<MyTcpSocket*>::Iterator iter=m_tcpSocketList.begin();
    for(;iter !=m_tcpSocketList.end();iter++)
    {
        if(mysocket==*iter)
        {
            (*iter)->deleteLater();//延迟释放空间
            *iter=NULL;
            m_tcpSocketList.erase(iter);
            break;
        }
    }
    for(int i=0;i<m_tcpSocketList.size();++i)
    {
        QString temp=m_tcpSocketList.at(i)->get_name();
        qDebug()<<temp;
    }
}
