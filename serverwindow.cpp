#include "serverwindow.h"
#include "ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    ui->startGameBtn->setEnabled(false);
    ui->defaultEdit->setText("11451");
    this->setWindowTitle("红棋连接窗口");
    this->setWindowIcon(QIcon(":/red_ma.png"));

    game = new onlineGameWindow;
    game->setWindowTitle("在线对局-红方");
    game->setWindowIcon(QIcon(":/red_shi.png"));

    connect(ui->startGameBtn,&QPushButton::clicked,[=](){
        game->myPlayerColor = 0;
        this->hide();
        game->show();

        connect(game,&onlineGameWindow::sendNetMsg,[=](){
            QString seed = QString("%1\n%2\n%3\n%4").arg(game->tempBeginerX).arg(game->tempBeginerY).arg(game->tempEnderX).arg(game->tempEnderY);
            socket->write(seed.toUtf8());
        });
    });

    connect(ui->actionexit,&QAction::triggered,[=](){
        emit this->netChoiceWindowBack();
    });

    connect(server,&QTcpServer::newConnection,[=](){
        socket = server->nextPendingConnection();
        ui->startGameBtn->setEnabled(true);
        ui->plainTextEdit->appendPlainText("客户端接入了！");
        //监测接收数据
        connect(socket,&QTcpSocket::readyRead,this,[=](){
            //接收client数据
            QByteArray temp;
            for(int i = 0 ; i < 4 ; i++){
                temp = socket->readLine();
                int a = temp.toInt();
                if(i == 0){
                    game->manager->beginer.setX(a);
                }else if(i == 1){
                    game->manager->beginer.setY(a);
                }else if(i == 2){
                    game->manager->ender.setX(a);
                }else{
                    game->manager->ender.setY(a);
                }
            }
            game->goChess();
            game->thisColor=1;
            game->update();
        });

        connect(socket,&QTcpSocket::disconnected,this,[=](){
            ui->startGameBtn->setEnabled(false);
            socket->close();
        });
    });
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::on_startBtn_clicked()
{
    unsigned short port = ui->defaultEdit->text().toUShort();
    server->listen(QHostAddress::Any,port);
    ui->startBtn->setEnabled(false);
}

