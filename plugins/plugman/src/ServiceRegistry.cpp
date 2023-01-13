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
#include "ServiceRegistry.h"
#include "PluginRegistry.h"

Q_LOGGING_CATEGORY(PLUGIN_SERVICE, "sageteamail.plugman.service")

ServiceRegistry::ServiceRegistry(QObject *parent) : QObject(parent)
{
    connect(this, &ServiceRegistry::serviceKeyChanged, this, &ServiceRegistry::loadServices);
}

ServiceRegistry::~ServiceRegistry()
{
    clear();
}

void ServiceRegistry::setServiceKey(const QString &key)
{
    m_key = key;
    loadServices();
}

void ServiceRegistry::startServices()
{
    for (auto service : m_services) {
        service->start();
    }
}

void ServiceRegistry::killServices()
{
    for (auto service : m_services) {
        service->stop();
    }
}

void ServiceRegistry::clear()
{
    killServices();
    qDeleteAll(m_services);
    m_services.clear();
}

void ServiceRegistry::sendMessage(const QString &pluginId, const QString &key, const QVariant &value)
{
    for (auto service : m_services) {
        if (service->pluginId() == pluginId) {
            service->messageReceived(key, value);
            return;
        }
    }
}

void ServiceRegistry::loadServices()
{
    auto plugins = PluginRegistry::instance()->getByLocation(m_key);
    for (auto plugin : plugins) {
        qCDebug(PLUGIN_SERVICE) << "Found Service plugin " << plugin->pluginId();
        if (auto servicePlugin = qobject_cast<ServicePlugin *>(plugin)) {
            qCDebug(PLUGIN_SERVICE) << "We have a service plugin!!!" << servicePlugin->location();
            m_services.append(servicePlugin);
        }
    }
}
