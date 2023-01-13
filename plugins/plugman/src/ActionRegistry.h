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
#ifndef ACTIONREGISTRY_H
#define ACTIONREGISTRY_H

#include <QLoggingCategory>
#include <QObject>
#include <QQmlIncubator>
#include <QQmlListProperty>
#include "PluginIncubator.h"


Q_DECLARE_LOGGING_CATEGORY(PLUGIN_ACTION)

class ActionRegistry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<QObject> defaultActions READ defaultActions)
    Q_PROPERTY(QList<QObject *> actions READ actions NOTIFY actionsChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)

public:
    explicit ActionRegistry(QObject *parent = 0);
    ~ActionRegistry();

    QQmlListProperty<QObject> defaultActions();
    QList<QObject *> actions() const;
    QString location() const;

signals:
    void actionsChanged(QList<QObject *> actions);
    void locationChanged(QString location);

public slots:
    void setLocation(QString location);

private slots:
    void loadActions();
    void finishLoading();
    void handleError();

private:
    QList<QObject *> m_actions;
    QList<QObject *> m_defaults;
    QString m_location;
    IncubatorList m_incubators;
};

#endif // ACTIONREGISTRY_H
