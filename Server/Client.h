#ifndef CLIENT_H
#define CLIENT_H
#include<QtNetwork/QTcpSocket>
#include<QString>
#define _MAX_NICK_NAME_SIZE 256

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
    QString nickname;
signals:

public slots:
};

#endif // CLIENT_H
