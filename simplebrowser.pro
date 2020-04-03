TEMPLATE = app
TARGET = simplebrowser
QT += webenginewidgets

DESTDIR = ../bin

HEADERS += \
    browser.h \
    browserwindow.h \
    tabwidget.h \
    singlecase.h \
    helper.h \
    webview.h \
    Settings.h \
    DragTabBar.h \
    FramelessMainWindow.h \
    TitleButtonGrouproup.h \
    Defs.h

SOURCES += \
    browser.cpp \
    browserwindow.cpp \
    main.cpp \
    tabwidget.cpp \
    webview.cpp \
    Settings.cpp \
    DragTabBar.cpp \
    FramelessMainWindow.cpp \
    TitleButtonGroup.cpp

FORMS += \

RESOURCES += data/simplebrowser.qrc
win32 {
    RC_ICONS = win_app.ico
}

macx{
    ICON = mac_app.icns
}

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/webenginewidgets/simplebrowser
#INSTALLS += target
