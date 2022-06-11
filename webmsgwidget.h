#ifndef WEBMSGWIDGET_H
#define WEBMSGWIDGET_H

#include <QWidget>

namespace Ui {
class webMsgWidget;
}

class webMsgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit webMsgWidget(QWidget *parent = nullptr);
    ~webMsgWidget();

private:
    Ui::webMsgWidget *ui;
};

#endif // WEBMSGWIDGET_H
