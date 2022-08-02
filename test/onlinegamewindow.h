#ifndef ONLINEGAMEWINDOW_H
#define ONLINEGAMEWINDOW_H

#include "gamewindow.h"

class onlineGameWindow : public gameWindow
{
    Q_OBJECT
public:
    onlineGameWindow();
    bool yesGo();

    bool myPlayerColor;//0红1黑
    void mousePressEvent(QMouseEvent * ev);
    void winGame();
    void goChess();

    QPushButton * upLoadBtn;
    int tempBeginerX;
    int tempBeginerY;
    int tempEnderX;
    int tempEnderY;
signals:
    void sendNetMsg();
};

#endif // ONLINEGAMEWINDOW_H
