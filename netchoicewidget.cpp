#include "netchoicewidget.h"
#include<QPushButton>
#include<QMessageBox>
netChoiceWidget::netChoiceWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowIcon(QIcon(":/black_pao.png"));
    this->setWindowTitle("选择阵营");
    this->setFixedSize(320,588);

    QPushButton * RedBtn = new QPushButton(this);
    QPushButton * BlackBtn = new QPushButton(this);
    QPushButton * ExitBtn = new QPushButton(this);

    ExitBtn->resize(150,40);
    ExitBtn->setText("返回");
    ExitBtn->move(80,0.5*(this->height()-ExitBtn->height())+100);

    BlackBtn->resize(150,40);
    BlackBtn->setText("选择黑棋");
    BlackBtn->move(80,0.5*(this->height()-BlackBtn->height())-100);

    RedBtn->resize(150,40);
    RedBtn->setText("选择红棋");
    RedBtn->move(80,0.5*(this->height()-BlackBtn->height()));

    connect(ExitBtn,&QPushButton::clicked,[=](){
       emit this->backMainWindow();
    });

    connect(RedBtn,&QPushButton::clicked,[=](){
        server = new ServerWindow;
        this->hide();
        server->show();

        connect(server,&ServerWindow::netChoiceWindowBack,[=](){
            server->close();
            this->show();
        });
    });
    connect(BlackBtn,&QPushButton::clicked,[=](){
        client = new ClientWindow;
        this->hide();
        client->show();

        connect(client,&ClientWindow::netChoiceWindowBack,[=](){
            client->close();
            this->show();
        });
    });
}
