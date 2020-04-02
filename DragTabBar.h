#ifndef TabTitleBar_H
#define TabTitleBar_H

#include <QTabBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QMainWindow>

class DragTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit DragTabBar(QMainWindow* mainWindow, QWidget *parent = nullptr);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    QPoint m_start;
    QPoint m_end;
    bool m_leftButtonPressed;

    QMainWindow *m_mainWindow = nullptr;
};

#endif // TabTitleBar_H
