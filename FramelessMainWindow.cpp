#include "FramelessMainWindow.h"
#ifdef Q_OS_WIN
#include "Windows.h"
#endif
#include <QDebug>
#include <QPainter>

#ifndef GET_X_LPARAM
    #define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif

#ifndef GET_Y_LPARAM
    #define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

FramelessMainWindow::FramelessMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

#ifdef Q_OS_WIN
bool FramelessMainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    bool re = false;
    if ("windows_generic_MSG" == eventType)
    {
        re = winEvent((MSG*)message, result);
    }
    return re;
}

bool FramelessMainWindow::winEvent(MSG *message, long *result)
{
    bool bre = true;
    do
    {
        if (WM_NCHITTEST != message->message)
        {
            bre = false;
            break;
        }

        int xPos = GET_X_LPARAM(message->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(message->lParam) - this->frameGeometry().y();

        if (xPos <= m_leftPadding && xPos >= 0 && yPos <= m_topPadding && yPos >= 0)
        {
            *result = HTTOPLEFT;
            break;
        }

        if (xPos >= (width() - m_rightPadding) && xPos < width() && yPos <= m_topPadding && yPos >= 0)
        {
            *result = HTTOPRIGHT;
            break;
        }

        if (xPos <= m_leftPadding && xPos >= 0 && yPos >= (this->height() - m_bottomPadding) && yPos <= height())
        {
            *result = HTBOTTOMLEFT;
            break;
        }

        if (xPos >= (this->width() - m_rightPadding) && xPos <= width() && yPos >= (this->height() - m_bottomPadding) && yPos <= height())
        {
            *result = HTBOTTOMRIGHT;
            break;
        }

        if (xPos <= m_leftPadding && xPos >= 0)
        {
            *result = HTLEFT;
            break;
        }

        if (xPos >= (this->width() - m_rightPadding) &&  xPos <= width())
        {
            *result = HTRIGHT;
            break;
        }

        if (yPos < m_topPadding && yPos >= 0)
        {
            *result = HTTOP;
            break;
        }

        if (yPos>= (this->height() - m_bottomPadding) && yPos <= height())
        {
            *result = HTBOTTOM;
            break;
        }

        bre = false;
    }while(false);

    return bre;
}
#endif

void FramelessMainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(139, 139, 139));
    painter.drawLine(0, 0, this->width() - 1, 0);
    painter.drawLine(0, 0, 0, this->height() - 1);
    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);
    QMainWindow::paintEvent(event);
}
