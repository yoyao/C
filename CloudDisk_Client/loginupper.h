#ifndef LOGINUPPER_H
#define LOGINUPPER_H

#include <QWidget>

namespace Ui {
class LoginUpper;
}

class LoginUpper : public QWidget
{
    Q_OBJECT

public:
    explicit LoginUpper(QWidget *parent = 0);
    ~LoginUpper();
protected:
   virtual void mouseMoveEvent(QMouseEvent *event);
   virtual void mousePressEvent(QMouseEvent *ev);

signals:
  void SwitchTag(int index);

private slots:
    void on_tbtn_close_clicked();

private:
    Ui::LoginUpper *ui;
     QPoint m_pt;    // 差值: 鼠标当前位置 - 窗口左上角点
    QWidget* m_parent;
};

#endif // LOGINUPPER_H
