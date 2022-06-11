#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QFile>
#include"gamewindow.h"
#include "netchoicewidget.h"
#include"webmsgwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    gameWindow * game;
    netChoiceWidget * net;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent * ev);
    webMsgWidget * webmsg;
private:
    Ui::MainWindow *ui;



};
#endif // MAINWINDOW_H
