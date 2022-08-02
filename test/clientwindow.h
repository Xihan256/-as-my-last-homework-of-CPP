#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include<QTcpSocket>
#include<onlinegamewindow.h>
namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();
    onlineGameWindow * game;
private:
    Ui::ClientWindow *ui;
    QTcpSocket * socket;
signals:
    void netChoiceWindowBack();
private slots:
    void on_connectBtn_clicked();
};

#endif // CLIENTWINDOW_H
