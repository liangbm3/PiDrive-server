#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

class OperateDB : public QObject
{
    Q_OBJECT
public:
    explicit OperateDB(QObject *parent = nullptr);
    static OperateDB& getInstance();
    void init();
    ~OperateDB();
    bool handleRegist(const char *name ,const char * pwd);
    bool handleLogin(const char *name ,const char* pwd);
    bool handleOffline(const char *name);
signals:

public slots:
private:
    QSqlDatabase m_db;
};

#endif // OPERATEDB_H
