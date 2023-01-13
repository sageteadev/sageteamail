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
#include "ActionRegistry.h"
#include <QtQml>
#include <QQmlError>
#include <QQmlEngine>
#include <QQmlComponent>
#include "PluginRegistry.h"

Q_LOGGING_CATEGORY(PLUGIN_ACTION, "sageteamail.plugman.action")

ActionRegistry::ActionRegistry(QObject *parent) : QObject(parent)
{
    connect(this, &ActionRegistry::locationChanged, this, &ActionRegistry::loadActions);
}

ActionRegistry::~ActionRegistry()
{
    qDeleteAll(m_actions);
}

QList<QObject *> ActionRegistry::actions() const
{
    return m_actions;
}

QString ActionRegistry::location() const
{
    return m_location;
}

QQmlListProperty<QObject> ActionRegistry::defaultActions()
{
    return QQmlListProperty<QObject>(this, m_defaults);
}

void ActionRegistry::setLocation(QString location)
{
    if (m_location == location)
        return;

    m_location = location;
    emit locationChanged(location);
}

void ActionRegistry::loadActions()
{
    qCDebug(PLUGIN_ACTION) << "Loading actions for: " << m_location;
    if (!m_actions.isEmpty()) {
        qDeleteAll(m_actions);
        m_actions.clear();
    }
    m_actions << m_defaults;
    auto plugins = PluginRegistry::instance()->getByLocation(m_location);
    for (auto plugin : plugins) {
        if (auto dp = qobject_cast<SageteaMailPlugin *>(plugin)) {
            PluginIncubator *incubator = new PluginIncubator(this);
            connect(incubator, &PluginIncubator::objectReady, this, &ActionRegistry::finishLoading);
            connect(incubator, &PluginIncubator::error, this, &ActionRegistry::handleError);
            incubator->setSourceUrl(qmlEngine(this), QUrl::fromLocalFile(dp->component()));
            m_incubators << incubator;
        }
    }
    emit actionsChanged(m_actions);
}

void ActionRegistry::finishLoading()
{
    PluginIncubator *incubator = qobject_cast<PluginIncubator *>(sender());
    if (incubator->status() == QQmlIncubator::Ready) {
        QObject *action = incubator->object();
        QByteArray cname(action->metaObject()->className());
        if (cname == QByteArrayLiteral("UCAction")) {
            qCDebug(PLUGIN_ACTION) << "Finished incubating";
            m_actions << action;
            emit actionsChanged(m_actions);
            qCDebug(PLUGIN_ACTION) << "Action loaded for " << m_location;
        } else {
            qCWarning(PLUGIN_ACTION) << "Plugin component not of type UCAction. Not going to use this";
            action->deleteLater();
            incubator->deleteLater();
        }
    }
}

void ActionRegistry::handleError()
{
    PluginIncubator *incubator = qobject_cast<PluginIncubator *>(sender());
    for (auto error : incubator->errors()) {
        qCDebug(PLUGIN_ACTION) << "Incubator Error: " << error.toString();
    }
}

