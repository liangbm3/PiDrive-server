#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include "protocol.h"
#include <QTcpSocket>
#include "operatedb.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = nullptr);
    QString get_name()
    {
        return m_strName;
    }

signals:
    void offline(MyTcpSocket *my_socket);
public slots:
    void recvMsg();
    void clientOffline();
private:
    QString m_strName;
};

PDU* handleRegistRequest(PDU* pdu);
PDU* handleLoginRequest(PDU* pdu);
#endif // MYTCPSOCKET_H
