#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#define SIDE_FILL 50
#define TOP_FILL 30
#define RIGHT_SIDE 200
#define BLOCK_W 100
#define BLOCK_H 80


#include <QMainWindow>
#include"mypoint.h"
#include"gamemanager.h"
#include<QSoundEffect>
#include<QPushButton>
class gameWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit gameWindow(bool isLoad = 0,QWidget *parent = nullptr);
    void paintEvent(QPaintEvent * ev);
    void initPoint();
    bool yesGo();
    void goChess();
    void mousePressEvent(QMouseEvent * ev);
    void myswap(myPoint &a,myPoint &b);

    bool jv();
    bool ma();
    bool xiang();
    bool shi();
    bool jiang();
    bool pao();
    bool zu();

    void saveGame();
    void winGame();

    myPoint chessBoard[10][9];
    bool thisColor = 0;
    gameManager * manager;
    bool isLoadGame = 0;
    myPoint tempBeginerPoint;
    myPoint tempEnderPoint;
    int pointSeedArray[4]={-1,-1,-1,-1};
    int clickedTime=0;
    QPushButton * lastBtn ;
private:


signals:
    void mainWindowBack();
};

#endif // GAMEWINDOW_H
