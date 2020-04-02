#include "DragTabBar.h"
#include <QMainWindow>
#include <QDebug>

DragTabBar::DragTabBar(QMainWindow *mainWindow, QWidget *parent)
    : QTabBar(parent),
    m_mainWindow(mainWindow) {

}

void DragTabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    //m_maximizeButton->click();
    QTabBar::mouseDoubleClickEvent(event);
}

void DragTabBar::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = true;
        m_start = event->globalPos();
    }
    QTabBar::mousePressEvent(event);
}

void DragTabBar::mouseMoveEvent(QMouseEvent *event)
{
    if (m_leftButtonPressed)
    {
        m_mainWindow->move(m_mainWindow->pos() + event->globalPos() - m_start);

        m_start = event->globalPos();
    }

    QTabBar::mouseMoveEvent(event);
}

void DragTabBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_leftButtonPressed = false;
    }

    QTabBar::mouseReleaseEvent(event);
}
