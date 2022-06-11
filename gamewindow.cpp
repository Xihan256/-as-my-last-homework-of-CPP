#include "gamewindow.h"


#include<QPainter>
#include<QDebug>
#include<QMenuBar>
#include<QMessageBox>
#include<QMouseEvent>
#include<cmath>
#include<QPushButton>
#include<QDateTime>
#include<QFileDialog>

gameWindow::gameWindow(bool isLoad,QWidget *parent)
    : QMainWindow{parent}
{
    manager = new gameManager(0);
    this->isLoadGame = isLoad;

    lastBtn = new QPushButton(this);
    lastBtn->resize(80,80);
    lastBtn->setText("悔棋\n(只能悔一步)");
    lastBtn->move(SIDE_FILL+8*BLOCK_W+80,0.5*this->height()-30);
    lastBtn->setEnabled(true);

    connect(lastBtn,&QPushButton::clicked,[=](){

        if(manager->beginer.x() != -1 and manager->beginer.y() != -1 and manager->ender.x() != -1 and manager->ender.y() != -1 ){
            if(this->thisColor == 1){
                this->thisColor = 0;
            }else{
                this->thisColor = 1;
            }
        }else{
            if(isLoadGame){
                manager->beginer.setX(this->pointSeedArray[0]);
                manager->beginer.setY(this->pointSeedArray[1]);
                manager->ender.setX(this->pointSeedArray[2]);
                manager->ender.setY(this->pointSeedArray[3]);
            }
        }

        if(isLoadGame){
            if(tempBeginerPoint.occupyed or tempEnderPoint.occupyed){
                if(this->thisColor == 1){
                    this->thisColor = 0;
                }else{
                    this->thisColor = 1;
                }
            }
        }

        chessBoard[manager->ender.y()][manager->ender.x()] = this->tempEnderPoint;
        chessBoard[manager->beginer.y()][manager->beginer.x()] = this->tempBeginerPoint;

        if(tempBeginerPoint.occupyed or tempEnderPoint.occupyed){
            lastBtn->setEnabled(false);
        }
        update();
    });

    this->setWindowTitle("本地对战");
    this->setWindowIcon(QIcon(":/red_jiang.png"));
    this->setFixedSize(1100,800);

    setPalette(QPalette(QColor(249, 214, 91)));
    setAutoFillBackground(true);

    QMenuBar * menu = new QMenuBar(this);
    setMenuBar(menu);
    QMenu * choice = menu->addMenu("选项");
    QAction * save = choice->addAction("保存");
    QAction * exit = choice->addAction("退出");

    connect(exit,&QAction::triggered,[=](){
        emit this->mainWindowBack();
    });

    connect(save,&QAction::triggered,this,&gameWindow::saveGame);

    initPoint();//初始化棋盘
}

void gameWindow::saveGame(){
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString pathName = time.toString("yyyy-MM-dd"); //设置显示格式

    QString path=QFileDialog::getSaveFileName(this,"保存文件",pathName,"TXT(*.txt)");
    QFile file(path);

    bool ret = file.open(QIODevice::WriteOnly);
    if(ret==false)
    {
        QMessageBox::critical(this,"出错了！","未保存或保存时出现未知错误");
        return;
    }

    for(int i=0;i<10;i++){
        for(int j=0;j<9;j++){
            QString seed = QString("%1\n%2\n%3\n").arg(chessBoard[i][j].color).arg(chessBoard[i][j].index).arg(chessBoard[i][j].occupyed);
            file.write(seed.toUtf8());
        }
    }
    QString whosGoing = QString("%1\n").arg(this->thisColor);
    file.write(whosGoing.toUtf8());

    QString tempBeginerPointInfo = QString("%1\n%2\n%3\n%4\n%5\n").arg(tempBeginerPoint.color).arg(tempBeginerPoint.index).arg(tempBeginerPoint.occupyed).arg(tempBeginerPoint.x).arg(tempBeginerPoint.y);
    file.write(tempBeginerPointInfo.toUtf8());
    QString tempEnderPointInfo = QString("%1\n%2\n%3\n%4\n%5\n").arg(tempEnderPoint.color).arg(tempEnderPoint.index).arg(tempEnderPoint.occupyed).arg(tempEnderPoint.x).arg(tempEnderPoint.y);
    file.write(tempEnderPointInfo.toUtf8());
    QString pointInfo = QString("%1\n%2\n%3\n%4").arg(manager->beginer.x()).arg(manager->beginer.y()).arg(manager->ender.x()).arg(manager->ender.y());
    file.write(pointInfo.toUtf8());

    file.close();
}

void gameWindow::winGame(){
    if(chessBoard[manager->ender.y()][manager->ender.x()].color==1){
        QMessageBox::information(this,"结束！","红方胜利");
    }else{
        QMessageBox::information(this,"结束！","黑方胜利");
    }

    _sleep(1200);
    emit this->mainWindowBack();
}

void gameWindow::mousePressEvent(QMouseEvent * ev){
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
        }
    }


}

bool gameWindow::jv(){
    if (manager->beginer.x() != manager->ender.x() and manager->beginer.y() != manager->ender.y()) {
            return false;
        }
        if (manager->beginer.x() == manager->ender.x()) {
            if (manager->beginer.y() > manager->ender.y()) {
                for (int i = manager->beginer.y()-1; i != manager->ender.y(); i--) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        return false;
                    }
                }
            }
            else {
                for (int i = manager->beginer.y()+1; i != manager->ender.y(); i++) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        return false;
                    }
                }
            }
        }

        if (manager->beginer.y() ==manager->ender.y()) {
            if (manager->beginer.x() > manager->ender.x()) {
                for (int i = manager->beginer.x()-1; i != manager->ender.x(); i--) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        return false;
                    }
                }
            }
            else {
                for (int i = manager->beginer.x()+1; i != manager->ender.x(); i++) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        return false;
                    }
                }
            }
        }
        return true;
}

bool gameWindow::ma(){
    if (abs(manager->beginer.x() - manager->ender.x()) > 2 or abs(manager->beginer.y() - manager->ender.y()) > 2) {
            return false;
        }
        if (abs(manager->beginer.x() - manager->ender.x()) == 2 and abs(manager->beginer.y() - manager->ender.y()) == 2) {
            return false;
        }
        if (abs(manager->beginer.x() - manager->ender.x()) <= 2 and abs(manager->beginer.y() - manager->ender.y()) == 0) {
            return false;
        }
        if (abs(manager->beginer.y() - manager->ender.y()) <= 2 and abs(manager->beginer.x() - manager->ender.x()) == 0) {
            return false;
        }
        if (abs(manager->beginer.x() - manager->ender.x()) == 1 and abs(manager->beginer.y() - manager->ender.y()) == 1) {
            return false;
        }

        if (manager->ender.x() > manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {
            if (chessBoard[manager->ender.y() - 1][manager->ender.x() - 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() > manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {
            if (chessBoard[manager->ender.y() + 1][manager->ender.x() - 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {
            if (chessBoard[manager->ender.y() - 1][manager->ender.x() + 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() <manager-> beginer.x() and manager->ender.y() < manager->beginer.y()) {
            if (chessBoard[manager->ender.y() + 1][manager->ender.x() + 1].occupyed) {
                return false;
            }
        }
        return true;
}

bool gameWindow::xiang(){
    if (!(abs(manager->beginer.x() - manager->ender.x()) == 2 and abs(manager->beginer.y() - manager->ender.y()) == 2)) {
            return false;
        }
        if (thisColor == 0) {
            if (manager->ender.y() >=5) {
                return false;
            }
        }
        if(thisColor==1) {
            if (manager->ender.y() <=4) {
                return false;
            }
        }

        if (manager->ender.x() > manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {
            if (chessBoard[manager->ender.y() - 1][manager->ender.x() - 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() > manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {
            if (chessBoard[manager->ender.y() + 1][manager->ender.x() - 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {
            if (chessBoard[manager->ender.y() - 1][manager->ender.x() + 1].occupyed) {
                return false;
            }
        }
        else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {
            if (chessBoard[manager->ender.y() + 1][manager->ender.x() + 1].occupyed) {
                return false;
            }
        }
        return true;
}

bool gameWindow::shi(){
    if (!(abs(manager->beginer.x() - manager->ender.x()) == 1 and abs(manager->beginer.y() - manager->ender.y()) == 1)) {
            return false;
        }
        if (abs(manager->ender.x() - 4) > 1) {
            return false;
        }
        if (thisColor == 0) {
            if (manager->ender.y() >=3) {
                return false;
            }
        }
        else {
            if (manager->ender.y() <=6) {
                return false;
            }
        }
        return true;
}

bool gameWindow::jiang(){
    if (abs(manager->beginer.x() - manager->ender.x()) > 1 or abs(manager->beginer.y() - manager->ender.y()) > 1) {
        return false;
    }
    if (abs(manager->ender.x() - 4) > 1) {
        return false;
    }
    if (manager->ender.x() > manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() > manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() <manager-> beginer.y()) {

        return false;

    }

    if (thisColor == 0) {
        if (manager->ender.y() >= 3) {
            return false;
        }
    }
    else {
        if (manager->ender.y() <= 6) {
            return false;
        }
    }

    if(manager->ender.y() < 3){
        for(int i=manager->ender.y();i<10;i++){
            if(chessBoard[i][manager->ender.x()].index == 4){
                bool temp = true;
                for(int j=manager->ender.y();j<i;j++){
                    if(chessBoard[j][manager->ender.x()].occupyed!=0){
                        temp=false;
                    }
                }
                if(temp){
                    return false;
                }
            }
        }
    }else if(manager->ender.y()>6){
        for(int i=manager->ender.y();i>=0;i--){
            if(chessBoard[i][manager->ender.x()].index == 4){
                bool temp = true;
                for(int j=manager->ender.y();j>i;j--){
                    if(chessBoard[j][manager->ender.x()].occupyed!=0){
                        temp=false;
                    }
                }
                if(temp){
                    return false;
                }
            }
        }
    }

    return true;
}

bool gameWindow::pao(){
    if (manager->beginer.x() != manager->ender.x() and manager->beginer.y() !=manager-> ender.y()) {
        return false;
    }

    if (chessBoard[manager->ender.y()][manager->ender.x()].occupyed) {
        if (manager->beginer.x() == manager->ender.x()) {
            if (manager->beginer.y() > manager->ender.y()) {
                int n = 0;
                for (int i = manager->beginer.y() - 1; i != manager->ender.y(); i--) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        n++;
                    }
                }
                if (!(n == 1 and chessBoard[manager->ender.y()][manager->ender.x()].color != chessBoard[manager->beginer.y()][manager->beginer.x()].color)) {
                    return false;
                }
            }
            else {
                int n = 0;
                for (int i = manager->beginer.y() + 1; i != manager->ender.y(); i++) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        n++;
                    }
                }
                if (!(n == 1 and chessBoard[manager->ender.y()][manager->ender.x()].color != chessBoard[manager->beginer.y()][manager->beginer.x()].color)) {
                    return false;
                }
            }
        }

        if (manager->beginer.y() == manager->ender.y()) {
            if (manager->beginer.x() > manager->ender.x()) {
                int n = 0;
                for (int i = manager->beginer.x() - 1; i != manager->ender.x(); i--) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        n++;
                    }
                }
                if (!(n == 1 and chessBoard[manager->ender.y()][manager->ender.x()].color != chessBoard[manager->beginer.y()][manager->beginer.x()].color)) {
                    return false;
                }
            }
            else {
                int n = 0;
                for (int i = manager->beginer.x() + 1; i != manager->ender.x(); i++) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        n++;
                    }
                }
                if (!(n == 1 and chessBoard[manager->ender.y()][manager->ender.x()].color != chessBoard[manager->beginer.y()][manager->beginer.x()].color)) {
                    return false;
                }
            }
        }
    }
    else {
        if (manager->beginer.x() == manager->ender.x()) {
            if (manager->beginer.y() > manager->ender.y()) {
                for (int i = manager->beginer.y() - 1; i != manager->ender.y(); i--) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        return false;
                    }
                }
            }
            else {
                for (int i = manager->beginer.y() + 1; i != manager->ender.y(); i++) {
                    if (chessBoard[i][manager->beginer.x()].occupyed) {
                        return false;
                    }
                }
            }
        }

        if (manager->beginer.y() == manager->ender.y()) {
            if (manager->beginer.x() > manager->ender.x()) {
                for (int i = manager->beginer.x() - 1; i != manager->ender.x(); i--) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        return false;
                    }
                }
            }
            else {
                for (int i = manager->beginer.x() + 1; i != manager->ender.x(); i++) {
                    if (chessBoard[manager->beginer.y()][i].occupyed) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

bool gameWindow::zu(){
    if (abs(manager->beginer.x() - manager->ender.x()) > 1 or abs(manager->beginer.y() - manager->ender.y()) > 1) {
        return false;
    }
    if (manager->ender.x() > manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() > manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() > manager->beginer.y()) {

        return false;

    }
    else if (manager->ender.x() < manager->beginer.x() and manager->ender.y() < manager->beginer.y()) {

        return false;

    }

    if (thisColor == 1) {
        if (manager->ender.y() >manager-> beginer.y()) {
            return false;
        }

        if (manager->beginer.y() > 4) {
            if (manager->ender.y() == manager->beginer.y()) {
                return false;
            }
        }
    }

    if (thisColor == 0) {
        if (manager->ender.y() < manager->beginer.y()) {
            return false;
        }
        if (manager->beginer.y() <5) {
            if (manager->ender.y ()== manager->beginer.y()) {
                return false;
            }
        }
    }
    return true;
}

bool gameWindow::yesGo(){//0黑1红

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

void gameWindow::myswap(myPoint &a,myPoint &b){
    int tempX=0;
    int tempY=0;

    tempX=a.x;
    a.x=b.x;
    b.x=tempX;

    tempY=a.y;
    a.y=b.y;
    b.y=tempY;
}

void gameWindow::goChess(){

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

void gameWindow::initPoint(){
    for(int i=0;i<10;i++){
        for(int j=0;j<9;j++){
            myPoint p(i,j,-1,-1,0);
            chessBoard[i][j] = p;
        }
    }
    //0123456车马相士将炮卒

    for(int i=0;i<9;i++){
        if(i<=4){
            chessBoard[0][i].occupyed = true; chessBoard[0][i].index = i; chessBoard[0][i].color = 1;
            chessBoard[9][i].occupyed = true; chessBoard[9][i].index = i; chessBoard[9][i].color = 0;
        }else{
            chessBoard[0][i].occupyed = true; chessBoard[0][i].index = 8-i; chessBoard[0][i].color = 1;
            chessBoard[9][i].occupyed = true; chessBoard[9][i].index = 8-i; chessBoard[9][i].color = 0;
        }

    }

    chessBoard[2][1].occupyed = true; chessBoard[2][1].index = 5; chessBoard[2][1].color = 1;
    chessBoard[2][7].occupyed = true; chessBoard[2][7].index = 5; chessBoard[2][7].color = 1;
    chessBoard[7][1].occupyed = true; chessBoard[7][1].index = 5; chessBoard[7][1].color = 0;
    chessBoard[7][7].occupyed = true; chessBoard[7][7].index = 5; chessBoard[7][7].color = 0;

    for (int i = 0; i < 9; i++) {
        if (i == 0 or i % 2 == 0) {
            chessBoard[3][i].occupyed = true; chessBoard[3][i].index = 6; chessBoard[3][i].color = 1;
            chessBoard[6][i].occupyed = true; chessBoard[6][i].index = 6; chessBoard[6][i].color = 0;
        }
    }

}


void gameWindow::paintEvent(QPaintEvent * ev){
    QPainter painter(this);
    QPen pen;

    painter.drawRect(SIDE_FILL,TOP_FILL,800,720);//宽800 长720  横8格 纵9格
    painter.drawRect(SIDE_FILL-5,TOP_FILL-5,810,730);

    for (int i = 1; i <= 7; i++) {
        painter.drawLine(SIDE_FILL + i * BLOCK_W, TOP_FILL, SIDE_FILL + i * BLOCK_W, 720 + TOP_FILL);
    }

    for (int j = 1; j <= 8; j++) {
        painter.drawLine(SIDE_FILL, TOP_FILL + j * BLOCK_H, 1100 - SIDE_FILL - RIGHT_SIDE, TOP_FILL + j * BLOCK_H);
    }

    painter.setRenderHint(QPainter::Antialiasing);
    QBrush brush(QColor(249, 214, 91));

    painter.drawLine(SIDE_FILL + 3 * BLOCK_W, TOP_FILL, SIDE_FILL + 5 * BLOCK_W, TOP_FILL + 2 * BLOCK_H);
    painter.drawLine(SIDE_FILL + 3 * BLOCK_W, TOP_FILL + 9 * BLOCK_H, SIDE_FILL + 5 * BLOCK_W, TOP_FILL + 7 * BLOCK_H);
    painter.drawLine(SIDE_FILL + 5 * BLOCK_W, TOP_FILL, SIDE_FILL + 3 * BLOCK_W, TOP_FILL + 2 * BLOCK_H);
    painter.drawLine(SIDE_FILL + 5 * BLOCK_W, TOP_FILL + 9 * BLOCK_H, SIDE_FILL + 3 * BLOCK_W, TOP_FILL + 7 * BLOCK_H);

    painter.setBrush(brush);
    painter.drawRect(SIDE_FILL + 1, TOP_FILL + 4 * BLOCK_H + 1,  8 * BLOCK_W , BLOCK_H );

    brush.setColor(QColor(Qt::black));
    painter.setBrush(brush);

    painter.drawEllipse(QPoint(SIDE_FILL+BLOCK_W,TOP_FILL+BLOCK_H*2),10,10);
    painter.drawEllipse(QPoint(SIDE_FILL+BLOCK_W*7,TOP_FILL+BLOCK_H*2),10,10);
    painter.drawEllipse(QPoint(SIDE_FILL+BLOCK_W,TOP_FILL+BLOCK_H*7),10,10);
    painter.drawEllipse(QPoint(SIDE_FILL+BLOCK_W*7,TOP_FILL+BLOCK_H*7),10,10);

    for(int i=0;i<5;i++){
        painter.drawEllipse(QPoint(SIDE_FILL+2*i*BLOCK_W,TOP_FILL+3*BLOCK_H),10,10);
        painter.drawEllipse(QPoint(SIDE_FILL+2*i*BLOCK_W,TOP_FILL+6*BLOCK_H),10,10);
    }

    //1黑，0红
    for(int i = 0 ; i < 10 ; i++){
        for(int j = 0 ; j < 9 ; j++){
            if(chessBoard[i][j].occupyed == 1){
                if(chessBoard[i][j].color == 1){
                    switch (chessBoard[i][j].index) {
                    case 0:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_jv.png"));
                        break;
                    }
                    case 1:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_ma.png"));
                        break;
                    }
                    case 2:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_xiang.png"));
                        break;
                    }
                    case 3:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_shi.png"));
                        break;
                    }
                    case 4:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_jiang.png"));
                        break;
                    }
                    case 5:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_pao.png"));
                        break;
                    }
                    case 6:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/black_zu.png"));
                        break;
                    }
                    default:{
                        break;
                    }
                    }

                }else{
                    switch (chessBoard[i][j].index) {
                    case 0:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_ju.png"));
                        break;
                    }
                    case 1:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_ma.png"));
                        break;
                    }
                    case 2:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_xiang.png"));
                        break;
                    }
                    case 3:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_shi.png"));
                        break;
                    }
                    case 4:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_jiang.png"));
                        break;
                    }
                    case 5:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_pao.png"));
                        break;
                    }
                    case 6:{
                        painter.drawPixmap(QPoint(j*BLOCK_W+SIDE_FILL-25,i*BLOCK_H+TOP_FILL-25),QPixmap(":/red_zu .png"));
                        break;
                    }
                    default:{
                        break;
                    }
                    }
                }
            }
        }

    }

    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(30);
    painter.setFont(font);
    if(this->thisColor==0){
        pen.setColor(Qt::black);
        painter.setPen(pen);
        painter.drawText(QPoint(SIDE_FILL+8*BLOCK_W+30,0.5*this->height()),"黑方行棋");
    }else{
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.drawText(QPoint(SIDE_FILL+8*BLOCK_W+30,0.5*this->height()),"红方行棋");
    }


}
