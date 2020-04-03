/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QApplication>
#include <QWebEngineSettings>
#include <QMutex>
#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextCodec>
#include "Settings.h"
#ifdef Q_OS_WIN
#include "helper.h"
#endif

extern "C" void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;

    case QtWarningMsg:
        text = QString("Warning:");
        break;

    case QtCriticalMsg:
        text = QString("Critical:");
        break;

    case QtFatalMsg:
        text = QString("Fatal:");
    }
    /* The QMessageLogContext class provides additional information about a log message.
    The class provides information about the source code location a qDebug(), qInfo(), qWarning(), qCritical() or qFatal() message was generated.
    Note: By default, this information is recorded only in debug builds. You can overwrite this explicitly by defining QT_MESSAGELOGCONTEXT or QT_NO_MESSAGELOGCONTEXT
    */
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(current_date).arg(msg);

    QDir dirLog = QCoreApplication::applicationDirPath();
#ifdef Q_OS_MACOS
    dirLog.cdUp();
#endif
    dirLog.cdUp();
    QString strLogPath = dirLog.absolutePath() + QString("/log");
    if(!dirLog.exists(strLogPath))
    {
        dirLog.mkdir(strLogPath);
    }
    dirLog.cd("log");
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString strLogFile = dirLog.absoluteFilePath(QString("log_")+currentTime+QString(".txt"));
    QFile file(strLogFile);
    file.open(QIODevice::WriteOnly| QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream.setCodec("utf-8");
    text_stream << message << "\r\n";
    file.flush();
    file.close();

    mutex.unlock();
}

int main(int argc, char **argv)
{
#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(ExceptionFilter);
#endif

    QCoreApplication::setOrganizationName("教研平台");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QApplication app(argc, argv);
#ifdef Q_OS_WIN
    app.setWindowIcon(QIcon("win_app.ico"));
#else
    app.setWindowIcon(QIcon("mac_app.icns"));
#endif

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QTextCodec::setCodecForLocale(codec);

    qInstallMessageHandler(outputMessage);

    QWebEngineSettings* setting = QWebEngineSettings::defaultSettings();
    setting->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    setting->setAttribute(QWebEngineSettings::AutoLoadImages, true);
    setting->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    setting->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
    setting->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    setting->setAttribute(QWebEngineSettings::LinksIncludedInFocusChain, true);
    setting->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    setting->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    setting->setAttribute(QWebEngineSettings::Accelerated2dCanvasEnabled, true);
    setting->setAttribute(QWebEngineSettings::WebGLEnabled,true);
    QWebEngineProfile::defaultProfile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    QUrl url = QUrl(Settings::getInstance()->getMainPage());

    Browser browser;
    BrowserWindow *window = browser.createWindow();
    window->tabWidget()->setUrl(url);

    return app.exec();
}
