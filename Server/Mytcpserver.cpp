#include "Mytcpserver.h"

char* namerandom(){
    srand(time(NULL));
    char name[_MAX_NICK_NAME_SIZE];
    for (int i=0;i<10;i++)
        name[i]=(char)rand()%100+25;
    return name;
}

Mytcpserver::Mytcpserver(QObject *parent) : QTcpServer(parent)
{
    client_list.clear();
    blacklist.clear();

    QFile file("./blacklist");

    if (!file.exists()) file.open(QIODevice::WriteOnly);
    else file.close();

}

Mytcpserver::~Mytcpserver(){
    while (!client_list.isEmpty()){
        delete client_list.front().socket;
        client_list.pop_front();
    }
}

void Mytcpserver::start_server(quint16 port){
    std::cout<<"[INFO] Start server..."<<std::endl;

    std::cout<<"[INFO] Reading blacklist ..."<<std::endl;

    QFile file("./blacklist");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) std::cout<<"[WARN] Can't read the blacklist!"<<std::endl;
    std::cout<<"[INFO] Find the file"<<std::endl;

    QString temp;
    while (!file.atEnd()){
         temp=file.readLine();
         blacklist.push_back(temp);
    }

    file.close();

    this->listen(QHostAddress::Any,port);
    if (!this->isListening()){
        std::cout<<"[ERRO] Listen error..."<<std::endl;
        return;
    }
    else {
        std::cout<<"[INFO] Listen at port:"<<port<<std::endl;
    }

    connect(this,SIGNAL(newConnection()),this,SLOT(on_newConnection()));

    connect(this,SIGNAL(acceptError(QAbstractSocket::SocketError)),
            this,SLOT(on_acceptError(QAbstractSocket::SocketError)));
}

void Mytcpserver::on_newConnection(){

    //qDebug()<<"on_newConnection()";

    if (this->hasPendingConnections()){
        std::cout<<"[INFO] Get a new connection"<<std::endl;
        //new_client=new Client(this);
        new_client.socket=this->nextPendingConnection();
        if (is_in_blacklist(new_client.socket)) return;
        //qDebug()<<"[INFO] client:"<<new_client.socket;b
        //qDebug()<<new_client.socket->state();


        if (!new_client.socket->waitForReadyRead()){
            std::cout<<"[INFO] no client nickname "<<std::endl;
            new_client.nickname=namerandom();
        }
        else{
            new_client.nickname= new_client.socket->readAll().data();
        }


        //qDebug()<<new_client->state();
        qDebug()<<"[INFO] Client :"<<new_client.nickname<<" connected";

        connect(new_client.socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
        connect(new_client.socket,SIGNAL(disconnected()),this,SLOT(on_disconnected()));
        client_list.push_back(new_client);
    }
}

void Mytcpserver::on_acceptError(QAbstractSocket::SocketError){
    std::cout<<"[ERRO] Accept error"<<std::endl;
}

void Mytcpserver::on_readyRead(){
    QTcpSocket* new_sender=(QTcpSocket*)QObject::sender();
    qDebug()<<"[INFO] on_readyRead "<<new_sender;

    for (int i=0;i<client_list.size();i++)
        if (client_list[i].socket==new_sender) {message=client_list[i].nickname+':'; break;}

    message.append(new_sender->readAll());

    qDebug()<<new_sender->peerAddress().toString()<<":"<<message;
    for (int i=0;i<client_list.size();i++){
        client_list[i].socket->write(message.toUtf8().toStdString().data());
        /*
        if (!client_list[i]->waitForBytesWritten()){
            std::cout<<"[ERRO] "<<client_list[i]->nickname.toStdString()<<" write error"<<std::endl;
        }
        */
    }
    qDebug()<<"[INFO] on_readyRead finish";

}


void Mytcpserver::on_disconnected(){
    QTcpSocket* disconnected_socket=(QTcpSocket*)QObject::sender();

    for (QVector<CLIENT>::iterator iter=client_list.begin();iter!=client_list.end();iter++)
        if (iter->socket==disconnected_socket){
            message=iter->nickname;
            iter->socket->deleteLater();
            client_list.erase(iter);
            break;
        }

    qDebug()<<"[INFO] "<<disconnected_socket<<"|"<<message<<" disconnected";


}

bool Mytcpserver::is_in_blacklist(QTcpSocket* param){
    for (QVector<QString>::iterator iter=blacklist.begin();iter!=blacklist.end();iter++){
        if (param->peerAddress().toString()==iter) return true;
    }
    return false;
}
