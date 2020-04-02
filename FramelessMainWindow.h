#ifndef FramelessMainWindow_H
#define FramelessMainWindow_H

#include <QMainWindow>

class FramelessMainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit FramelessMainWindow(QWidget *parent = 0);

    protected:
#ifdef Q_OS_WIN
        virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;
        bool winEvent(MSG *message, long *result);
#endif

        void paintEvent(QPaintEvent *event) override;

    private:
        int m_leftPadding = 5;
        int m_topPadding = 5;
        int m_rightPadding = 5;
        int m_bottomPadding = 5;
};

#endif // FramelessMainWindow_H
