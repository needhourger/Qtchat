#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    nickname.clear();
    socket=new QTcpSocket(this);
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_clicked(bool)));
    connect(ui->actionNick_name,SIGNAL(triggered(bool)),this,SLOT(on_actionNick_name_triggered(bool)),Qt::UniqueConnection);
    connect(ui->actionServer,SIGNAL(triggered(bool)),this,SLOT(on_actionServer_triggered(bool)),Qt::UniqueConnection);

    emit(ui->actionNick_name->triggered());
    emit(ui->actionServer->triggered());
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}

void MainWindow::on_actionNick_name_triggered(bool){
    srand(time(NULL));
    bool ok;

    nickname=QInputDialog::getText(this,"Setting","Pleas input your nickname",QLineEdit::Normal,"",&ok);

    if (ok){
        if (nickname.isEmpty()){
            QMessageBox::warning(this,"Warn","You haven't set your nickname");
            emit(ui->actionNick_name->triggered());
        }
        else qDebug()<<"Nickname="<<nickname;
    }
    else {
        QMessageBox::warning(this,"Warn","You haven't set your nickname"
                             "rand one for you");
        for (int i=0;i<10;i++) nickname.append((char)rand()%100+25);
        QMessageBox::information(this,"your name:",nickname);
        qDebug()<<"Nickname="<<nickname;
    }
}

void MainWindow::on_actionServer_triggered(bool){

    server_addr=QInputDialog::getText(this,"Setting","Pleas input server address:",QLineEdit::Normal,"127.0.0.1");
    port=QInputDialog::getInt(this,"Setting","Please input Port(0-65535):",1234,0,65535);

    qDebug()<<"Server address="<<server_addr;
    qDebug()<<"Port="<<port;

    try_connect(server_addr,port);
}


void MainWindow::on_clicked(bool){
    QString temp;
    temp=ui->textEdit->toPlainText();

    if (temp.isEmpty()) return;

    socket->write(temp.toUtf8().toStdString().data());
    if (!socket->waitForBytesWritten()){
        qDebug()<<"Send error";
        ui->textBrowser->append("Send error:"+temp);
        qDebug()<<socket->state();
    }
    else{
        qDebug()<<"Send success";
        qDebug()<<socket->state();
    }
    ui->textEdit->clear();
}

void MainWindow::try_connect(QString addr,quint16 port){
    socket->disconnectFromHost();
    if (!socket->waitForDisconnected()){
        QMessageBox::information(this,"Warn","failed to disconnect form the host");
    }

    socket->connectToHost(addr,port);
    if (socket->waitForConnected()){
        socket->write(nickname.toStdString().data());
        if (socket->waitForBytesWritten()){
            QMessageBox::information(this,"INFO","Connected");
            qDebug()<<socket->state();
            connect(socket,SIGNAL(readyRead()),this,SLOT(on_readyRead()));
        }
        else {
            QMessageBox::warning(this,"Warn","Connected,but nick name sending failed");
            socket->disconnectFromHost();
            if (!socket->waitForDisconnected()) qDebug()<<"Disconnect error";
        }
    }
    else {
        QMessageBox::warning(this,"Warn","connect failed!"
                             "Please check the server setting");
    }

}

void MainWindow::on_readyRead(){
    QString message=socket->readAll().data();
    ui->textBrowser->append(message);
}
