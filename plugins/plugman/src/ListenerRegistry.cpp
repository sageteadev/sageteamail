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
#include "ListenerRegistry.h"
#include <QQmlEngine>
#include "PluginRegistry.h"

Q_LOGGING_CATEGORY(PLUGIN_LISTENER, "sageteamail.plugman.listener")

ListenerRegistry::ListenerRegistry(QObject *parent) : QObject(parent)
{
}

QQmlListProperty<QQuickItem> ListenerRegistry::defaultListeners()
{
    return QQmlListProperty<QQuickItem>(this, m_defaults);
}

QQuickItem *ListenerRegistry::createListenerFromURl(const QString &url)
{
    if (url.isEmpty()) {
        qCWarning(PLUGIN_LISTENER) << "Invalid component url";
        return Q_NULLPTR;
    }
    auto engine = qmlEngine(this);
    QQmlComponent itemComponent(engine, QUrl::fromLocalFile(url));
    if (itemComponent.isError()) {
        for (auto error : itemComponent.errors()) {
            qCDebug(PLUGIN_LISTENER) << "Failed loading plugin with error:";
            qCDebug(PLUGIN_LISTENER) << error.toString();
        }
        return Q_NULLPTR;
    }
    return qobject_cast<QQuickItem *>(itemComponent.create(engine->contextForObject(this)));
}

void ListenerRegistry::componentComplete()
{
    auto plugins = PluginRegistry::instance()->getByLocation(QStringLiteral("SageteaMail::Listener"));
    for (auto plugin : plugins) {
        if (auto dp = qobject_cast<SageteaMailPlugin *>(plugin)) {
            qCDebug(PLUGIN_LISTENER) << "Loading plugin: " << dp->pluginId();
            QQuickItem *item = createListenerFromURl(dp->component());
            if (item) {
                QByteArray cname(item->metaObject()->className());
                if (cname == QByteArrayLiteral("QFAppListener")) {
                    m_plugins << item;
                } else {
                    qCWarning(PLUGIN_LISTENER) << "Listener Plugin not of type QFAppListener. Instead it's" << cname;
                    item->deleteLater();
                }
            }
        }
    }
}

