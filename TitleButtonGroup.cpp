#include "TitleButtonGrouproup.h"
#include <QHBoxLayout>
#include <QEvent>
#include <QVariant>
#include <QApplication>
#include "Defs.h"

TitleButtonGroup::TitleButtonGroup(QMainWindow *mainWindow, QWidget *parent)
    : QWidget(parent),
      m_mainWindow(mainWindow)
{
    m_minimizeButton = new QPushButton(this);
    m_minimizeButton->setFixedSize(TITLE_BUTTON_WIDTH, 20);
    m_minimizeButton->setObjectName("minimizeButton");
    m_minimizeButton->setStyleSheet("QPushButton{border-image:url(:/images/app_window_minimize.png);}"
                                  "QPushButton:hover{border-image:url(:/images/app_window_minimize_hover.png);}"
                                  "QPushButton:pressed{border-image:url(:/images/app_window_minimize_active.png);}");

    m_maximizeButton = new QPushButton(this);
    m_maximizeButton->setFixedSize(TITLE_BUTTON_WIDTH, 20);
    m_maximizeButton->setObjectName("maximizeButton");
    m_maximizeButton->setStyleSheet("QPushButton{border-image:url(:/images/app_window_maximize.png);}"
                                  "QPushButton:hover{border-image:url(:/images/app_window_maximize_hover.png);}"
                                  "QPushButton:pressed{border-image:url(:/images/app_window_maximize_active.png);}");

    m_closeButton = new QPushButton(this);
    m_closeButton->setFixedSize(TITLE_BUTTON_WIDTH, 20);
    m_closeButton->setObjectName("closeButton");
    m_closeButton->setStyleSheet("QPushButton{border-image:url(:/images/app_window_close.png);}"
                                  "QPushButton:hover{border-image:url(:/images/app_window_close_hover.png);}"
                                  "QPushButton:pressed{border-image:url(:/images/app_window_close_active.png);}");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    QHBoxLayout* hLayout = new QHBoxLayout;
    layout->addLayout(hLayout);
    layout->addStretch(1);
    hLayout->setMargin(0);
    hLayout->setSpacing(0);
#ifdef Q_OS_WIN
    hLayout->addStretch(1);
    hLayout->addWidget(m_minimizeButton);
    hLayout->addWidget(m_maximizeButton);
    hLayout->addWidget(m_closeButton);
#else
    hLayout->addWidget(m_closeButton);
    hLayout->addWidget(m_minimizeButton);
    hLayout->addWidget(m_maximizeButton);
    hLayout->addStretch(1);
#endif
    setLayout(layout);

    connect(m_minimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_maximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_closeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));

    mainWindow->installEventFilter(this);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

bool TitleButtonGroup::eventFilter(QObject *obj, QEvent *event)
{
    switch(event->type())
    {
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void TitleButtonGroup::updateMaximize()
{
    QWidget *pWindow = m_mainWindow;
    if (pWindow)
    {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize)
        {
            m_maximizeButton->setToolTip(tr("恢复"));
            m_maximizeButton->setProperty("maximizeProperty", "restore");

            m_maximizeButton->setStyleSheet("QPushButton{border-image:url(:/images/app_window_restore.png);}"
                                          "QPushButton:hover{border-image:url(:/images/app_window_restore_hover.png);}"
                                          "QPushButton:pressed{border-image:url(:/images/app_window_restore_active.png);}");
        }
        else
        {
            m_maximizeButton->setToolTip(tr("最大化"));
            m_maximizeButton->setProperty("maximizeProperty", "maximize");

            m_maximizeButton->setStyleSheet("QPushButton{border-image:url(:/images/app_window_maximize.png);}"
                                          "QPushButton:hover{border-image:url(:/images/app_window_maximize_hover.png);}"
                                          "QPushButton:pressed{border-image:url(:/images/app_window_maximize_active.png);}");
        }

        m_maximizeButton->setStyle(QApplication::style());
    }
}

void TitleButtonGroup::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = m_mainWindow;
    if (pWindow)
    {
        if (pButton == m_minimizeButton)
        {
            pWindow->showMinimized();
        }
        else if (pButton == m_maximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        }
        else if (pButton == m_closeButton)
        {
            pWindow->close();
        }
    }
}

void TitleButtonGroup::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setPen(QColor(139, 139, 139));
    painter.drawLine(0, this->height() - 3, this->width() - 1, this->height() - 3);
    QWidget::paintEvent(event);
}
