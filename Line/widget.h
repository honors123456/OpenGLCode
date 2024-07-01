#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    virtual void paintEvent(QPaintEvent* event) override;
public slots:
    void slotUpdateWin();
private:
    void DDALine(QPainter* painter,QPoint p1,QPoint p2);
private:
    Ui::Widget *ui;
    QPushButton* btn;
};
#endif // WIDGET_H
