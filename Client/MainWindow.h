#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <time.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void try_connect(QString addr="127.0.0.1",quint16 port=1234);
    ~MainWindow();

public slots:
    void on_clicked(bool);

    void on_actionNick_name_triggered(bool);
    void on_actionServer_triggered(bool);

    void on_readyRead();

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString nickname;
    QString server_addr;
    quint16 port;

};

#endif // MAINWINDOW_H
