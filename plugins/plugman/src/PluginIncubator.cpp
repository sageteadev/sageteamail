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
#include "PluginIncubator.h"
#include <QQmlContext>
#include <QDebug>

Q_LOGGING_CATEGORY(PLUGIN_INCUBATOR, "sageteamail.plugman.incubator")

PluginIncubator::PluginIncubator(QObject *parent) : QObject(parent),
    QQmlIncubator(QQmlIncubator::Asynchronous), m_comp(Q_NULLPTR)
{
}

void PluginIncubator::setSourceUrl(QQmlEngine *engine, const QUrl &source)
{
    m_comp = new QQmlComponent(engine, source, this);
    if (m_comp->isError()) {
        qCDebug(PLUGIN_INCUBATOR) << "Comp failed: " << m_comp->errorString();
        emit error();
        return;
    }
    QQmlContext *c = engine->contextForObject(parent());
    if (c->isValid()) {
        m_comp->create(*this, c);
    }
}

void PluginIncubator::statusChanged(QQmlIncubator::Status status)
{
    switch (status) {
    case QQmlIncubator::Null:
        break;
    case QQmlIncubator::Ready:
        qCDebug(PLUGIN_INCUBATOR) << "Incubator finished and ready to hatch.";
        emit objectReady();
        break;
    case QQmlIncubator::Loading:
        qCDebug(PLUGIN_INCUBATOR) << "Incubating plugin";
        break;
    case QQmlIncubator::Error:
        qCDebug(PLUGIN_INCUBATOR) << "Incubation Error";
        emit error();
        break;
    default:
        break;
    }
}
