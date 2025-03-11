#include "mytcpsocket.h"
#include <QDebug>


MyTcpSocket::MyTcpSocket(QObject *parent)
    : QTcpSocket{parent}
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));
    connect(this,SIGNAL(disconnected()),this,SLOT(clientOffline()));
}

void MyTcpSocket::recvMsg()
{
    qDebug()<<this->bytesAvailable();
    uint uiPUDlen=0;
    this->read((char*)&uiPUDlen,sizeof(uint));
    uint uiMsgLen=uiPUDlen-sizeof(PDU);
    PDU *pdu =mkPDU(uiMsgLen);
    this->read((char*)pdu+sizeof(uint),uiPUDlen-sizeof(int));
    PDU* respdu=NULL;
    switch (pdu->uiMsgType)
    {
    case MSG_TYPE_REGIST_REQUEST:
        respdu=handleRegistRequest(pdu);//请求处理
        break;
    case MSG_TYPE_LOGIN_REQUEST:
        respdu=handleLoginRequest(pdu);
    default:
        break;
    }
    if(NULL!=respdu)
    {
        qDebug()<<respdu->uiMsgType<<" "<<respdu->caFileData;
        this->write((char*)respdu,respdu->uiPDULen);
        free(respdu);
        respdu=NULL;
    }
    free(pdu);
    pdu=NULL;
    // qDebug()<<pdu->uiMsgType<<pdu->caFileData;
}

void MyTcpSocket::clientOffline()
{
    OperateDB::getInstance().handleOffline(m_strName.toStdString().c_str());
    emit offline(this);
}

PDU* handleRegistRequest(PDU* pdu)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    strncpy(caName,pdu->caFileData,32);
    strncpy(caPwd,pdu->caFileData+32,32);
    qDebug()<<caName<<caPwd<<pdu->uiMsgType;
    bool ret = OperateDB::getInstance().handleRegist(caName,caPwd);
    PDU *respdu=mkPDU(0);
    respdu->uiMsgType=MSG_TYPE_REGIST_RESPONE;
    if(ret)
    {
        strcpy(respdu->caFileData,REGIST_OK);
    }
    else
    {
        strcpy(respdu->caFileData,REGIST_FAILED);
    }
    return respdu;
}

PDU* handleLoginRequest(PDU* pdu)
{
    char caName[32]={'\0'};
    char caPwd[32]={'\0'};
    //拷贝读取的信息
    strncpy(caName,pdu->caFileData,32);
    strncpy(caPwd,pdu->caFileData+32,32);
    qDebug()<<pdu->uiMsgType<<" "<<caName<<" "<<caPwd;
    bool ret=OperateDB::getInstance().handleLogin(caName,caPwd);
    PDU *respdu=mkPDU(0);
    respdu->uiMsgType=MSG_TYPE_LOGIN_RESPONE;
    if(ret)
    {
        strcpy(respdu->caFileData,LOGIN_OK);
    }
    else
    {
        strcpy(respdu->caFileData,LOGIN_FAILED);
    }
    return respdu;
}
