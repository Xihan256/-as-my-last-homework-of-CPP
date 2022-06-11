#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>
#include<QPushButton>
#include<QMessageBox>
#include"gamewindow.h"
#include<QFileDialog>
#include"webmsgwidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/black_jiang.png"));
    this->setWindowTitle("SCUT象棋");
    this->setFixedSize(320,588);

    webmsg = new webMsgWidget;
    webmsg->hide();

    connect(ui->actionExit,&QAction::triggered,this,&QMainWindow::close);
    connect(ui->actionabout,&QAction::triggered,[=](){
        QMessageBox::information(this,"学生信息","3班王宏宇202130481388");
    });

    connect(ui->actionsupport,&QAction::triggered,[=](){
        QMessageBox::information(this,"打赏通道","欢迎打钱,微信:Why2905795198");
    });

    connect(ui->exitBtn,&QPushButton::clicked,this,&QMainWindow::close);

    connect(ui->startBtn,&QPushButton::clicked,[=](){
        game  = new gameWindow;
        this->hide();
        game->show();

        connect(game,&gameWindow::mainWindowBack,this,[=](){
            game->close();
            this->show();
        });
    });

    connect(ui->actionloCAL,&QAction::triggered,[=](){
        game  = new gameWindow;
        this->hide();
        game->show();

        connect(game,&gameWindow::mainWindowBack,this,[=](){
            game->close();
            this->show();
        });
    });

    connect(ui->actionOpen,&QAction::triggered,[=](){
        game  = new gameWindow(1);
        QString FilePath = QDir::currentPath();
        QString path = QFileDialog::getOpenFileName(this,"打开文件",FilePath,"TXT(*.txt)");

        QFile file(path);
        bool ret = file.open(QIODevice::ReadOnly);
        if(ret==false)
        {
            QMessageBox::critical(this,"出错了！","未选取或选取时出现未知错误");
            return;
        }

        QByteArray temp;
        int times=0;
        int numOfPoints=-1;
        while(!file.atEnd()){
            times++;
            temp = file.readLine();
            int a =temp.toInt();

            if(times % 3 ==1){
                numOfPoints++;
            }

            if(times<=270){
                if(times % 3 ==1){
                    game->chessBoard[numOfPoints/9][numOfPoints%9].color=a;
                }else if(times%3==2){
                    game->chessBoard[numOfPoints/9][numOfPoints%9].index=a;
                }else if(times%3==0){
                    game->chessBoard[numOfPoints/9][numOfPoints%9].occupyed=a;
                }
            }else{
                if(times == 271){
                    game->thisColor=(bool)a;
                }else if(times == 272){
                    game->tempBeginerPoint.color=a;
                }else if(times == 273){
                    game->tempBeginerPoint.index=a;
                }else if(times == 274){
                    game->tempBeginerPoint.occupyed=a;
                }else if(times == 275){
                    game->tempBeginerPoint.x=a;
                }else if(times == 276){
                    game->tempBeginerPoint.y=a;
                }else if(times == 277){
                    game->tempEnderPoint.color=a;
                }else if(times == 278){
                    game->tempEnderPoint.index=a;
                }else if(times == 279){
                    game->tempEnderPoint.occupyed=a;
                }else if(times == 280){
                    game->tempEnderPoint.x=a;
                }else if(times == 281){
                    game->tempEnderPoint.y=a;
                }else if(times == 282){
                    game->pointSeedArray[0]=a;
                }else if(times == 283){
                    game->pointSeedArray[1]=a;
                }else if(times == 284){
                    game->pointSeedArray[2]=a;
                }else{
                    game->pointSeedArray[3]=a;
                }
            }



        }

        this->hide();
        game->show();

        connect(game,&gameWindow::mainWindowBack,this,[=](){
            game->close();
            this->show();
        });
    });

    connect(ui->stratBtn_web,&QPushButton::clicked,[=](){


        net = new netChoiceWidget;
        this->hide();
        net->show();

        webmsg->show();

        connect(net,&netChoiceWidget::backMainWindow,[=](){
           net->close();
           this->show();
        });
    });

    connect(ui->actiononline,&QAction::triggered,[=](){
        net = new netChoiceWidget;
        this->hide();
        net->show();

        connect(net,&netChoiceWidget::backMainWindow,[=](){
           net->close();
           this->show();
        });
    });
}

void MainWindow::paintEvent(QPaintEvent * ev){
    QPainter painter(this);

    painter.drawPixmap(0,0.25*this->height(),320,320,QPixmap(":/scut.jpg"));

}

MainWindow::~MainWindow()
{
    delete ui;
}

