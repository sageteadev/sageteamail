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
#ifndef PLUGINREGISTRY_H
#define PLUGINREGISTRY_H

#include <QLoggingCategory>
#include <QObject>
#include <QMultiMap>
#include <QSharedPointer>
#include <QSettings>
#include "PluginLoader.h"

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_REGISTRY)

// key is plugin location i.e "Composer::ContextAction"
// we use a multimap as we may have multiple plugins for the same location
typedef QMultiMap<QString, PluginInfo *> Registry;

class PluginRegistry : public QObject
{
    Q_OBJECT
public:
    explicit PluginRegistry(QObject *parent = 0);
    ~PluginRegistry();

    static PluginRegistry *instance();

    Registry registry() const;
    bool supportsLocation(const QString &location);
    QStringList pluginLocations() const;
    void setPluginLocations(const QStringList &locations);
    void loadPlugins(const QStringList &paths = QStringList());

    QList<PluginInfo *> getAllPlugins(const bool enabledOnly = true);
    QList<PluginInfo *> getByLocation(const QString &key, const bool enabledOnly = true);

signals:
    void pluginsLoaded();

public slots:
    void enablePlugin(const QString &id);
    void disablePlugin(const QString &id);

protected:
    void updateRegistryConf();

private:
    Registry m_registry;
    QScopedPointer<PluginLoader> m_loader;
    QStringList m_locations;
    QSettings m_regConf;
};

#endif // PLUGINREGISTRY_H
