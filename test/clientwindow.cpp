#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    ui->defalutEdit->setText("11451");
    ui->startGameBtn->setEnabled(false);
    ui->disconnectBtn->setEnabled(false);
    socket = new QTcpSocket(this);
    this->setWindowIcon(QIcon(":/black_ma.png"));
    this->setWindowTitle("黑棋连接页面");

    game = new onlineGameWindow;
    game->setWindowTitle("在线对局-黑方");
    game->setWindowIcon(QIcon(":/black_shi.png"));

    connect(ui->startGameBtn,&QPushButton::clicked,[=](){
        game->myPlayerColor = 1;
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

    connect(socket,&QTcpSocket::connected,this,[=](){
        ui->startGameBtn->setEnabled(true);
        ui->connectBtn->setEnabled(false);
        ui->disconnectBtn->setEnabled(true);
        ui->plainTextEdit->appendPlainText("连接到服务器了！");
    });

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
        game->thisColor=0;
        game->update();
    });

    connect(socket,&QTcpSocket::disconnected,this,[=](){
        ui->startGameBtn->setEnabled(false);
        ui->connectBtn->setEnabled(true);
        ui->disconnectBtn->setEnabled(false);
        socket->close();
    });
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::on_connectBtn_clicked()
{
    QString ip = ui->locationEdit->text();
    unsigned short port = ui->defalutEdit->text().toUShort();

    socket->connectToHost(QHostAddress(ip),port);
}

