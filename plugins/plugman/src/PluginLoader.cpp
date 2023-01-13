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
#include "PluginLoader.h"
#include <QCoreApplication>
#include <QGuiApplication>
#include <QPluginLoader>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QLibrary>
#include <QStringBuilder>

Q_LOGGING_CATEGORY(PLUGIN_LOADER, "sageteamail.plugman.loader")

PluginLoader::PluginLoader()
{
    // by default only look next to the app binary for a plugins folder
    m_pluginPaths << qApp->applicationDirPath() % "/plugins";
}

PluginLoader::~PluginLoader()
{
    qDeleteAll(m_translators);
    m_translators.clear();
}

void PluginLoader::loadPlugins(QMultiMap<QString, PluginInfo *> &registry, const QString &pluginType)
{
    Q_UNUSED(pluginType);
    qCDebug(PLUGIN_LOADER) << "Loading plugins....";
    QStringList dirs;
    dirs << qApp->applicationDirPath() % "/plugins";
    dirs << m_pluginPaths;
    QString pluginEnv = QString(qgetenv("SAGETEAMAIL_PLUGINS"));
    if (!pluginEnv.isEmpty()) {
        dirs << pluginEnv;
    }

    // First iterate over library plugins
    QStringList pluginPaths;

    qCDebug(PLUGIN_LOADER) << "Looking for library plugins";
    Q_FOREACH(const QString &directory, dirs) {
        QDir dir(directory);
        Q_FOREACH(const QString &file, dir.entryList()) {
            const QString abs = QFileInfo(dir.absoluteFilePath(file)).canonicalFilePath();
            if (pluginPaths.contains(abs)) {
                continue;
            }
            pluginPaths << abs;
            if (!QLibrary::isLibrary(abs)) {
                continue;
            }
            QPluginLoader *l = new QPluginLoader(abs);
            if (l->load()) {
                if (auto plugin = qobject_cast<PluginInterface *>(l->instance())) {
                    if (PluginInfo *p = plugin->create(Q_NULLPTR)) {
                        qCDebug(PLUGIN_LOADER) << "+---------------------------------+";
                        qCDebug(PLUGIN_LOADER) << "Library plugin found >> " << plugin->name();
                        qCDebug(PLUGIN_LOADER) << "Registered >> " << p->location();
                        qCDebug(PLUGIN_LOADER) << "I18n >> " << p->i18n();
                        qCDebug(PLUGIN_LOADER) << "+---------------------------------+";
                        registry.insert(p->location(), p);
                        if (!p->i18n().isEmpty()) {
                            // we also load the translations for this plugin now
                            loadTranslations(directory % "/" % p->i18n());
                        }
                    }
                }
            }
            l->deleteLater();
        }
    }

    qCDebug(PLUGIN_LOADER) << "Looking for sageteamail spec plugins";
    // Now scan for any *.dekko-plugin specs and generate a DekkoPlugin
    pluginPaths.clear();

    Q_FOREACH(const QString &directory, dirs) {
        QDirIterator it(directory, QStringList() << "*.sageteamail-plugin", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString path = it.next();
            if (pluginPaths.contains(path)) {
                continue;
            }
            pluginPaths << path;

            if (auto pluginInfo = SageteaMailPlugin::fromSpec(path)) {
                qCDebug(PLUGIN_LOADER) << "+-------------------------------------+";
                qCDebug(PLUGIN_LOADER) << "PluginReady: " << path;
                qCDebug(PLUGIN_LOADER) << "Registered: " << pluginInfo->location();
                qCDebug(PLUGIN_LOADER) << "Path: " << it.fileInfo().absolutePath();
                qCDebug(PLUGIN_LOADER) << "I18n >> " << pluginInfo->i18n();
                qobject_cast<SageteaMailPlugin *>(pluginInfo)->setPluginDirectory(it.fileInfo().absolutePath());
                registry.insert(pluginInfo->location(), pluginInfo);
                if (!pluginInfo->i18n().isEmpty()) {
                    // we also load the translations for this plugin now
                    loadTranslations(it.fileInfo().absolutePath() % "/" % pluginInfo->i18n());
                }
                qCDebug(PLUGIN_LOADER) << "+-------------------------------------+";
            }
        }
    }
}

void PluginLoader::setPluginPaths(const QStringList &paths)
{
    if (!paths.isEmpty()) {
        m_pluginPaths = paths;
    }
}

void PluginLoader::loadTranslations(const QString &i18nDir)
{
    qCDebug(PLUGIN_LOADER) << "Plugin declared i18n directory: " << i18nDir;
    QDir dir(i18nDir);
    if (dir.exists()) {
        qCDebug(PLUGIN_LOADER) << "Plugin has i18n directory, creating a translator";
        QTranslator *tr = new QTranslator(Q_NULLPTR);
        if (tr->load(QLocale::system().name(), i18nDir)) {
            qApp->installTranslator(tr);
            m_translators.append(tr);
            qCDebug(PLUGIN_LOADER) << "Translator created for directory: " << i18nDir;
        } else {
            qCDebug(PLUGIN_LOADER) << "Failed creating translator for: " << i18nDir;
            qCDebug(PLUGIN_LOADER) << "Locale: " << QLocale::system().name();
        }
    } else {
        qCDebug(PLUGIN_LOADER) << "i18n directory doesn't exist";
    }
}


