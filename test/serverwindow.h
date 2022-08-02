#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include<QTcpServer>
#include<QTcpSocket>
#include"onlinegamewindow.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

    onlineGameWindow * game;

private:
    Ui::ServerWindow *ui;
    QTcpServer * server;
    QTcpSocket * socket;
signals:
    void netChoiceWindowBack();
private slots:
    void on_startBtn_clicked();
};

#endif // SERVERWINDOW_H
