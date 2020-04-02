#include "Settings.h"
#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QVariant>
#include "singlecase.h"

#define GENERATEINI
#define DEFAULT_MAINPAGE "https://www.baidu.com"

Settings *Settings::getInstance()
{
    return Singleton<Settings>::get();
}

QString Settings::getMainPage() const
{
    return getString("mainPage", "Addr", DEFAULT_MAINPAGE);
}

Settings::Settings(QObject *parent) : QObject(parent)
{
    QDir appDir = QApplication::applicationDirPath();
    appDir.cdUp();
#ifdef Q_OS_MACOS
    appDir.cdUp();
#endif
    if(!appDir.cd("config"))
    {
        appDir.mkdir(appDir.path() + "/config");
        appDir.cd("config");
    }

    m_qsettings = new QSettings(appDir.absoluteFilePath("config.ini"), QSettings::IniFormat, this);
    m_qsettings->setIniCodec("GBK");
}

QString Settings::getString(const QString &key,
                            const QString &group,
                            const QString &defaultValue) const
{
    return getValue(key, group, defaultValue).toString();
}

QVariant Settings::getValue(const QString &key,
                            const QString &group,
                            const QVariant &defaultValue) const
{
    if (!group.isEmpty()) {
        m_qsettings->beginGroup(group);
    }
    QVariant result = m_qsettings->value(key);

    if(result.isNull())
    {
#ifdef GENERATEINI
        m_qsettings->setValue(key,defaultValue);
        m_qsettings->sync();
#endif
        result = defaultValue;
    }

    if (!group.isEmpty()) {
        m_qsettings->endGroup();
    }

    return result;
}

Settings::~Settings() {
    if (m_qsettings) {
        m_qsettings->deleteLater();
    }
}
