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
#include "PluginRegistry.h"
#include <QPointer>
#include <QDebug>
#include <SnapStandardPaths.h>

Q_LOGGING_CATEGORY(PLUGIN_REGISTRY, "sageteamail.plugman.registry")

PluginRegistry::PluginRegistry(QObject *parent) : QObject(parent),
    m_loader(new PluginLoader),
    m_regConf(SnapStandardPaths::writableLocation(SnapStandardPaths::AppConfigLocation) +
              QStringLiteral("/PluginRegistry.conf"), QSettings::IniFormat)
{
    m_regConf.setValue("version", "0.1");
}

PluginRegistry::~PluginRegistry()
{
    qDeleteAll(m_registry);
    m_registry.clear();
}

static QPointer<PluginRegistry> s_registry;
PluginRegistry *PluginRegistry::instance()
{
    if (s_registry.isNull()) {
        qCDebug(PLUGIN_REGISTRY) << "Creating instance";
        s_registry = new PluginRegistry();
    }
    return s_registry;
}

Registry PluginRegistry::registry() const
{
    return m_registry;
}

bool PluginRegistry::supportsLocation(const QString &location)
{
    return m_locations.contains(location);
}

QStringList PluginRegistry::pluginLocations() const
{
    return m_locations;
}

void PluginRegistry::setPluginLocations(const QStringList &locations)
{
    m_locations = locations;
}

void PluginRegistry::loadPlugins(const QStringList &paths)
{
    qDeleteAll(m_registry);
    m_registry.clear();
    if (!m_loader.isNull()) {
        m_loader->setPluginPaths(paths);
        m_loader->loadPlugins(m_registry);
        updateRegistryConf();
    }
}

QList<PluginInfo *> PluginRegistry::getAllPlugins(const bool enabledOnly)
{
    if (enabledOnly) {
        QList<PluginInfo *> plugins;
        for (auto plugin : m_registry.values()) {
            m_regConf.beginGroup(plugin->pluginId());
            const bool enabled = m_regConf.value(QStringLiteral("enabled"), true).toBool();
            m_regConf.endGroup();
            if (enabled) {
                plugins << plugin;
            }
        }
        return plugins;
    }
    return m_registry.values();
}

QList<PluginInfo *> PluginRegistry::getByLocation(const QString &key, const bool enabledOnly)
{
    if (!m_locations.contains(key)) {
        qCWarning(PLUGIN_REGISTRY) << "Location not supported: " << key;
        return QList<PluginInfo *>();
    }
    if (enabledOnly) {
        qCDebug(PLUGIN_REGISTRY) << "Getting all enabled plugins";
        QList<PluginInfo *> plugins;
        for (auto plugin : m_registry.values(key)) {
            m_regConf.beginGroup(plugin->pluginId());
            bool enabled = m_regConf.value(QStringLiteral("enabled"), true).toBool();
            m_regConf.endGroup();
            if (enabled) {
                qCDebug(PLUGIN_REGISTRY) << "Plugin enabled: " << plugin->pluginId();
                enablePlugin(plugin->pluginId());
                plugins << plugin;
            }
        }
        return plugins;
    }
    return m_registry.values(key);
}

void PluginRegistry::enablePlugin(const QString &id)
{
    m_regConf.beginGroup(id);
    m_regConf.setValue("enabled", true);
    m_regConf.endGroup();
}

void PluginRegistry::disablePlugin(const QString &id)
{
    m_regConf.beginGroup(id);
    m_regConf.setValue("enabled", false);
    m_regConf.endGroup();
}

void PluginRegistry::updateRegistryConf()
{
    const QStringList keys = m_regConf.childGroups();
    // we enable unknown plugins by default
    for (auto plugin : m_registry.values()) {
        if (!keys.contains(plugin->pluginId())) {
            qCDebug(PLUGIN_REGISTRY) << plugin->pluginId() << "Not in" << keys;
            enablePlugin(plugin->pluginId());
        }
    }
    m_regConf.sync();
}

