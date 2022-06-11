#ifndef NETCHOICEWIDGET_H
#define NETCHOICEWIDGET_H

#include <QWidget>
#include"serverwindow.h"
#include"clientwindow.h"
class netChoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit netChoiceWidget(QWidget *parent = nullptr);

    ServerWindow * server;
    ClientWindow * client;
signals:
    void backMainWindow();
};

#endif // NETCHOICEWIDGET_H
