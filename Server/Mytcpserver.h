#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include<QtNetwork/QTcpServer>
#include<QtNetwork/QTcpSocket>
#include<QVector>
#include<QDebug>
#include<QString>
#include<iostream>
#include<time.h>
#include<QFile>
#include<QFileInfo>
#include"Client.h"

struct CLIENT{
    QTcpSocket* socket;
    QString nickname;
};

class Mytcpserver : public QTcpServer
{
    Q_OBJECT
public:
    explicit Mytcpserver(QObject *parent = nullptr);
    ~Mytcpserver();
    void start_server(quint16 port=1234);
    bool is_in_blacklist(QTcpSocket* param);

signals:

public slots:
    void on_newConnection();
    void on_acceptError(QAbstractSocket::SocketError);
    void on_readyRead();
    void on_disconnected();

private:
    QVector<CLIENT>client_list;
    QVector<QString>blacklist;
    CLIENT new_client;


    /*
    Client* new_client;
    Client* new_sender;
    */
    QString message;
};

#endif // MYTCPSERVER_H
