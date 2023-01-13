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
#include "QuickPlugger.h"
#include <QtQml/QtQml>
#include <QtQml/QQmlContext>
#include "ActionRegistry.h"
#include "ItemRegistry.h"
#include "ListenerRegistry.h"
#include "ServiceRegistry.h"


void QuickPlugger::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("PlugMan"));
    // @uri PlugMan
    qmlRegisterType<ActionRegistry>(uri, 1, 0, "ActionRegistry");
    qmlRegisterType<ItemRegistry>(uri, 1, 0, "ItemRegistry");
    qmlRegisterType<ListenerRegistry>(uri, 1, 0, "ListenerRegistry");
    qmlRegisterType<ServiceRegistry>(uri, 1, 0, "ServiceRegistry");
}

void QuickPlugger::initializeEngine(QQmlEngine *engine, const char *uri)
{
    QQmlExtensionPlugin::initializeEngine(engine, uri);
}
