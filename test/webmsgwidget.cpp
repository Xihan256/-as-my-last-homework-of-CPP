#include "webmsgwidget.h"
#include "ui_webmsgwidget.h"
#include<QIcon>
webMsgWidget::webMsgWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::webMsgWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("玩前须知(重要！)");
    this->setWindowIcon(QIcon(":/black_zu.png"));
}

webMsgWidget::~webMsgWidget()
{
    delete ui;
}
