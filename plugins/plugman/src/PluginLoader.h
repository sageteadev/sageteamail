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
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QLoggingCategory>
#include <QObject>
#include <QtPlugin>
#include "PluginInfo.h"
#include <QTranslator>

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_LOADER)

class PluginInterface
{
public:
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual PluginInfo *create(QObject *parent) const = 0;
};

Q_DECLARE_INTERFACE(PluginInterface, "PluginInterface")

class PluginLoader
{
public:
    explicit PluginLoader();
    ~PluginLoader();

    void loadPlugins(QMultiMap<QString, PluginInfo *> &registry, const QString &pluginType = QString());
    void setPluginPaths(const QStringList &paths);

protected:
    PluginInfo *parsePluginSpec(const QString &path);
    void initSettings(const QVariantMap &map);
    void loadTranslations(const QString &i18nDir);

private:
    QStringList m_pluginPaths;
    QList<QPointer<QTranslator> > m_translators;
};

#endif // PLUGINLOADER_H
