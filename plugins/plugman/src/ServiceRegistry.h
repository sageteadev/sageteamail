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
#ifndef SERVICEREGISTRY_H
#define SERVICEREGISTRY_H

#include <QLoggingCategory>
#include <QObject>
#include <QProcess>
#include "PluginInfo.h"

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_SERVICE)

class ServiceRegistry : public QObject
{
    Q_OBJECT
public:
    explicit ServiceRegistry(QObject *parent = 0);
    ~ServiceRegistry();

    // The location key used to filter out service plugins
    void setServiceKey(const QString &key);
    void startServices();
    void killServices();
    void clear();

    Q_INVOKABLE void sendMessage(const QString &pluginId, const QString &key, const QVariant &value);

signals:
    void serviceKeyChanged();

private slots:
    void loadServices();

private:
    QList<ServicePlugin *> m_services;
    QString m_key;
};

#endif // SERVICEREGISTRY_H
