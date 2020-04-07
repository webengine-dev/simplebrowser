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

#include "tabwidget.h"
#include <QMenu>
#include <QTabBar>
#include <QWebEngineProfile>
#include "Defs.h"
#include "DragTabBar.h"

TabWidget::TabWidget(QMainWindow *mainWindow, QWebEngineProfile *profile, QWidget *parent)
    : QTabWidget(parent)
    , m_profile(profile)
{
    //close,min,max buttons
    m_titleButtonGroup = new TitleButtonGroup(mainWindow, this);
    m_titleButtonGroup->setFixedSize(TITLE_BUTTONGROUP_WIDTH, TITLE_HEIGHT);

    DragTabBar *tabBar = new DragTabBar(mainWindow, this);
    this->setTabBar(tabBar);
    //QTabBar *tabBar = this->tabBar();
    tabBar->setTabsClosable(true);
    tabBar->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    tabBar->setMovable(true);
    tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &TabWidget::closeTab);

    setDocumentMode(true);
    setElideMode(Qt::ElideRight);

#ifdef Q_OS_WIN
    QString style = QString("QTabWidget::tab-bar{right: %1px;}").arg(TITLE_BUTTONGROUP_WIDTH);
#else
    QString style = QString("QTabWidget::tab-bar{left: %1px;}").arg(TITLE_BUTTONGROUP_WIDTH);
#endif
    setStyleSheet(style);
}

WebView *TabWidget::currentWebView() const
{
    return webView(currentIndex());
}

WebView *TabWidget::webView(int index) const
{
    return qobject_cast<WebView*>(widget(index));
}

void TabWidget::setupView(WebView *webView)
{
    connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
        int index = indexOf(webView);
        QString strTitle = title;
        if (index != -1) {
            if (title.indexOf("xdf.cn") != -1 || title.indexOf("about:blank") != -1
                    || title.indexOf("通行证") != -1) {
                strTitle = tr("教研平台");
            }
            setTabText(index, strTitle);
            setTabToolTip(index, strTitle);
        }
        if (currentIndex() == index)
            emit titleChanged(strTitle);
    });

    connect(webView, &QWebEngineView::loadFinished, [this, webView](bool ok) {
        int index = indexOf(webView);
        if (index != -1) {
            if (!ok) {
                //webView->setContent(QString("加载失败").toUtf8(), "text/html;charset=UTF-8");
                webView->setHtml("<body><center><div style='font-size:30px;color:red;'>请检查网络</div></center></body>");
            }
        }
    });
}

WebView *TabWidget::createTab()
{
    WebView *webView = createBackgroundTab();
    setCurrentWidget(webView);
    return webView;
}
#include <QWebEngineSettings>
WebView *TabWidget::createBackgroundTab()
{
    WebView *webView = new WebView;
    WebPage *webPage = new WebPage(m_profile, webView);
    webPage->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, false);
    webView->setPage(webPage);
    setupView(webView);
    int index = addTab(webView, tr(""));
    Q_UNUSED(index);
    //setTabIcon(index, webView->favIcon());
    // Workaround for QTBUG-61770
    webView->resize(currentWidget()->size());
    webView->show();
    return webView;
}

void TabWidget::closeTab(int index)
{
    if (WebView *view = webView(index)) {
        bool hasFocus = view->hasFocus();
        removeTab(index);
        if (hasFocus && count() > 0)
            currentWebView()->setFocus();
        if (count() == 0)
            createTab();
        view->deleteLater();
    }
}

void TabWidget::setUrl(const QUrl &url)
{
    if (WebView *view = currentWebView()) {
        view->setUrl(url);
        view->setFocus();
    }
}

void TabWidget::triggerWebPageAction(QWebEnginePage::WebAction action)
{
    if (WebView *webView = currentWebView()) {
        webView->triggerPageAction(action);
        webView->setFocus();
    }
}

void TabWidget::reloadTab(int index)
{
    if (WebView *view = webView(index))
        view->reload();
}

void TabWidget::resizeEvent(QResizeEvent *e)
{
#ifdef Q_OS_WIN
    m_titleButtonGroup->setGeometry(this->width()-m_titleButtonGroup->width(), 0, m_titleButtonGroup->width(), m_titleButtonGroup->height());
#else
    m_titleButtonGroup->setGeometry(0, 0, m_titleButtonGroup->width(), m_titleButtonGroup->height());
#endif
    QTabWidget::resizeEvent(e);
}
