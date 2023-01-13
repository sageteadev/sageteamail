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
#ifndef LISTENERREGISTRY_H
#define LISTENERREGISTRY_H

#include <QLoggingCategory>
#include <QObject>
#include <QQuickItem>
#include <QQmlParserStatus>

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_LISTENER)

class ListenerRegistry : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QQmlListProperty<QQuickItem> defaultListeners READ defaultListeners)
public:
    explicit ListenerRegistry(QObject *parent = 0);

    QQmlListProperty<QQuickItem> defaultListeners();

    virtual void classBegin() override {}
    virtual void componentComplete() override;
private:
    QQuickItem *createListenerFromURl(const QString &url);
    QList<QQuickItem *> m_defaults;
    QList<QQuickItem *> m_plugins;
};

#endif // LISTENERREGISTRY_H
