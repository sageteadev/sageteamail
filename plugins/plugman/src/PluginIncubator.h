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
#ifndef PLUGININCUBATOR_H
#define PLUGININCUBATOR_H

#include <QLoggingCategory>
#include <QObject>
#include <QQmlIncubator>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QUrl>

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_INCUBATOR)

class PluginIncubator : public QObject, public QQmlIncubator
{
    Q_OBJECT
public:
    explicit PluginIncubator(QObject *parent = 0);

    void setSourceUrl(QQmlEngine *engine, const QUrl &source);

signals:
    void objectReady();
    void error();

    // QQmlIncubator interface
protected:
    virtual void statusChanged(Status status) override;

private:
    QQmlComponent *m_comp;
};

typedef QList<PluginIncubator *> IncubatorList;

#endif // PLUGININCUBATOR_H
