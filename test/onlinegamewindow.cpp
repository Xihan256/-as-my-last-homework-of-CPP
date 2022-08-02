#include "onlinegamewindow.h"
#include"gamewindow.h"
#include<QPushButton>
#include<QMouseEvent>
#include<QMessageBox>
onlineGameWindow::onlineGameWindow()
{
    this->lastBtn->hide();

    upLoadBtn = new QPushButton(this);
    upLoadBtn->setText("提交");
    upLoadBtn->resize(80,80);
    upLoadBtn->move(SIDE_FILL+8*BLOCK_W+80,0.5*this->height()-80);

    connect(upLoadBtn,&QPushButton::clicked,[=](){
        emit this->sendNetMsg();
        upLoadBtn->setEnabled(false);
    });

}

void onlineGameWindow::winGame(){
    if(chessBoard[manager->ender.y()][manager->ender.x()].color==1){
        QMessageBox::information(this,"结束！","黑方胜利");
    }else{
        QMessageBox::information(this,"结束！","红方胜利");
    }

    _sleep(1200);
    emit this->mainWindowBack();
}

void onlineGameWindow::goChess(){

    this->lastBtn->setEnabled(true);

    this->tempBeginerPoint = chessBoard[manager->beginer.y()][manager->beginer.x()];
    this->tempEnderPoint = chessBoard[manager->ender.y()][manager->ender.x()];

    bool isWin=false;
    if(chessBoard[manager->ender.y()][manager->ender.x()].index == 4){
        isWin=true;
    }

    myswap(chessBoard[manager->beginer.y()][manager->beginer.x()],chessBoard[manager->ender.y()][manager->ender.x()]);
    chessBoard[manager->ender.y()][manager->ender.x()].occupyed=1;
    chessBoard[manager->beginer.y()][manager->beginer.x()].occupyed=0;

    chessBoard[manager->ender.y()][manager->ender.x()].color=chessBoard[manager->beginer.y()][manager->beginer.x()].color;
    chessBoard[manager->beginer.y()][manager->beginer.x()].color = -1;

    chessBoard[manager->ender.y()][manager->ender.x()].index=chessBoard[manager->beginer.y()][manager->beginer.x()].index;
    chessBoard[manager->beginer.y()][manager->beginer.x()].index=-1;

    chessBoard[manager->beginer.y()][manager->beginer.x()].index=-1;

    if(this->thisColor==0){
        this->thisColor=1;
    }else{
        this->thisColor=0;
    }

    update();

    if(isWin){
        this->winGame();
    }
}

void onlineGameWindow::mousePressEvent(QMouseEvent * ev){
    this->clickedTime++;

    int row = ev->x();
    int col = ev->y();

    bool a = (row - SIDE_FILL) % BLOCK_W >(0.5 * BLOCK_W);
    bool b = (col - TOP_FILL) % BLOCK_H > (0.5 * BLOCK_H);

    row = (row - SIDE_FILL) / BLOCK_W ;
    col = (col - TOP_FILL) / BLOCK_H;

    if (a == true and b == true) {
        row += 1;
        col += 1;
    }
    else if (a == true and b == false) {
        row += 1;
    }
    else if (a == false and b == true) {
        col += 1;
    }


    if(this->clickedTime % 2 != 0){
        if(chessBoard[col][row].occupyed==0){
            this->clickedTime=0;
            return;
        }

        manager->beginer.setX(row);
        manager->beginer.setY(col);

    }else{
        manager->ender.setX(row);
        manager->ender.setY(col);
        this->clickedTime = 0 ;

        if(manager->ender.x() == manager->beginer.x() and manager->beginer.y() == manager->ender.y()){
            this->clickedTime=1;
            return;
        }

        if(chessBoard[manager->ender.y()][manager->ender.x()].color ==  chessBoard[manager->beginer.y()][manager->beginer.x()].color){
            this->clickedTime=1;
            manager->beginer = manager->ender;
            return;
        }
        if(this->yesGo()){
            goChess();
            this->tempBeginerX = manager->beginer.x();
            this->tempBeginerY = manager->beginer.y();
            this->tempEnderX = manager->ender.x();
            this->tempEnderY = manager->ender.y();
            upLoadBtn->setEnabled(true);
        }
    }


}

bool onlineGameWindow::yesGo(){//0黑1红

    if(this->myPlayerColor == 0 and chessBoard[manager->beginer.y()][manager->beginer.x()].color == 1){
        return false;
    }

    if(this->myPlayerColor == 1 and chessBoard[manager->beginer.y()][manager->beginer.x()].color == 0){
        return false;
    }

    if(this->thisColor==0 and chessBoard[manager->beginer.y()][manager->beginer.x()].color==0){
        return false;
    }

    if(this->thisColor==1 and chessBoard[manager->beginer.y()][manager->beginer.x()].color==1){
        return false;
    }

    if(chessBoard[manager->ender.y()][manager->ender.x()].occupyed){
        if(chessBoard[manager->ender.y()][manager->ender.x()].color == chessBoard[manager->beginer.y()][manager->beginer.x()].color){
            return false;
        }
    }

    if(manager->ender.x()!=manager->beginer.x()){
        int tempNum=0;
        int firstJiangPos=0;
        int secondJiangPos=0;

        for(int i=0;i<10;i++){
            if(chessBoard[i][manager->beginer.x()].index==4){
                if(tempNum==0){
                    firstJiangPos=i;
                }else{
                    secondJiangPos=i;
                }
                tempNum++;
            }
        }
        int tempNumOfChess=0;
        if(tempNum==2){
            for(int i=firstJiangPos+1;i<secondJiangPos;i++){
                if(chessBoard[i][manager->beginer.x()].occupyed){
                    tempNumOfChess++;
                }
            }
        }

        if(tempNumOfChess == 1){
            if(chessBoard[manager->beginer.y()][manager->beginer.x()].index != 4){
                return false;
            }
        }
    }


    switch (chessBoard[manager->beginer.y()][manager->beginer.x()].index) {
    case 0:{
        return this->jv();
    }
    case 1:{
        return this->ma();
    }
    case 2:{
        return this->xiang();

    }
    case 3:{
        return this->shi();
    }
    case 4:{
        return this->jiang();
    }
    case 5:{
        return this->pao();
    }
    case 6:{
        return this->zu();
    }
    default:{
        return false;
    }
    }
}
