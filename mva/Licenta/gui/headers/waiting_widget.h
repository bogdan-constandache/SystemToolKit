#ifndef WAITING_WIDGET_H
#define WAITING_WIDGET_H

#include <QWidget>

namespace Ui {
class QWaitingWidget;
}

class QWaitingWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::QWaitingWidget  *ui;

public:
    explicit QWaitingWidget(QWidget *parent = 0);
    ~QWaitingWidget();
};

#endif // WAITING_WIDGET_H
