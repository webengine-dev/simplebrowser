#ifndef TITLEBUTTONGROUPROUP_H
#define TITLEBUTTONGROUPROUP_H

#include <QWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QPainter>

class TitleButtonGroup : public QWidget
{
    Q_OBJECT
public:
    explicit TitleButtonGroup(QMainWindow *mainWindow, QWidget *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event) override;
    void updateMaximize();

protected:
#ifdef Q_OS_WIN
    void paintEvent(QPaintEvent *event) override;
#endif

protected slots:
    void onClicked();

private:
    QPushButton* m_minimizeButton = nullptr;
    QPushButton* m_maximizeButton = nullptr;
    QPushButton* m_closeButton = nullptr;

    QMainWindow *m_mainWindow = nullptr;
};

#endif // TITLEBUTTONGROUPROUP_H
