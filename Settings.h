#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QVariant>

class QSettings;
class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings* getInstance();

    QString getMainPage() const;

    Settings(QObject *parent=0);
    ~Settings();

private:
    QVariant getValue(const QString &key,
                      const QString &group = QString(),
                      const QVariant &defaultValue = QVariant()) const;

    QString getString(const QString& key,
                      const QString& group = QString(),
                      const QString& defaultValue = QString()) const;

private:
    QSettings *m_qsettings = nullptr;
};

#endif // SETTINGS_H
