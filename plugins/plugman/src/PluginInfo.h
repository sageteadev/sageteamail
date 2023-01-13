/* Copyright (C) 2017 Dan Chapman <dpniel@ubuntu.com>

   This file is part of Dekko email client for Ubuntu devices

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef PLUGININFO_H
#define PLUGININFO_H

#include <QLoggingCategory>
#include <QObject>
#include <QVariantMap>
#include <QJsonObject>
#include <QDebug>

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_INFO)

class PluginInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pluginId READ pluginId CONSTANT)
    Q_PROPERTY(QString location READ location CONSTANT)
    Q_PROPERTY(QVariantMap documentation READ documentation CONSTANT)
    Q_PROPERTY(QVariantMap settings READ settings CONSTANT)

public:
    explicit PluginInfo(QObject *parent = 0);

    virtual QString pluginId() const = 0;
    virtual QString location() const = 0;
    virtual QString i18n() const = 0;
    virtual QVariantMap documentation() const = 0;
    virtual QVariantMap settings() const { return QVariantMap();}

};

class SageteaMailPlugin : public PluginInfo
{
    Q_OBJECT
    Q_PROPERTY(QString component READ component CONSTANT)
public:
    explicit SageteaMailPlugin(QObject *parent = 0);

    static SageteaMailPlugin* fromSpec(const QString &specPath, QObject *parent = 0);

    QString pluginId() const;
    QString location() const;
    QString component();
    QString i18n() const;
    QVariantMap documentation() const;
    QVariantMap settings() const;

    QString pluginPath() const;

    void setSpec(const QJsonObject &spec);
    void setPluginDirectory(const QString &path);

private:
    QJsonObject m_spec;
    QString m_pluginPath;
};

class ServicePlugin : public PluginInfo
{
    Q_OBJECT
public:
    explicit ServicePlugin(QObject *parent = 0);

    // PluginInfo interface
public:
    virtual QString pluginId() const override {
        qWarning() << "Not implemented here";
        return QString();
    }
    virtual QString location() const override {
        qWarning() << "Not implemented here";
        return QString();
    }
    virtual QString i18n() const override {
        qWarning() << "Not implemented here";
        return QString();
    }
    virtual QVariantMap documentation() const override {
        qWarning() << "Not implemented here";
        return QVariantMap();
    }

    virtual void start() {}
    virtual void stop() {}

    virtual void messageReceived(const QString &key, const QVariant &value);
};

#endif // PLUGININFO_H
