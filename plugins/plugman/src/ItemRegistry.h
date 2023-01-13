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
#ifndef ITEMREGISTRY_H
#define ITEMREGISTRY_H

#include <QLoggingCategory>
#include <QObject>
#include <QPointer>
#include <QQuickItem>
#include <QQmlListProperty>
#include "PluginIncubator.h"

Q_DECLARE_LOGGING_CATEGORY(PLUGIN_ITEM_REGISTRY)

class ItemRegistry : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QQmlListProperty<QQuickItem> defaultItems READ defaultItems)
    Q_PROPERTY(LoadMode loadMode READ loadMode WRITE setLoadMode NOTIFY loadModeChanged)
    Q_PROPERTY(QString pluginId MEMBER m_pluginId)
    // Should plugins be loaded asynchonously. Default items are always created synchronously
    Q_PROPERTY(bool asynchronous READ asynchronous WRITE setAsynchronous NOTIFY asyncChanged)
    Q_ENUMS(LoadMode)

public:
    explicit ItemRegistry(QObject *parent = 0);

    enum LoadMode {
        DefaultOnly,
        LoadFirstEnabled,
        LoadLastEnabled,
        LoadAll,
        LoadById // will fallback to default if a plugin with id doesn't exist.
    };

    QQuickItem *target() const;
    QString location() const;
    QQmlListProperty<QQuickItem> defaultItems();

    LoadMode loadMode() const;
    bool asynchronous() const;

    Q_INVOKABLE QString findFirstEnabled(const QString &location);

signals:
    void targetChanged(QQuickItem *target);
    void locationChanged(QString location);
    void loadModeChanged(LoadMode loadMode);

    void asyncChanged(bool asynchronous);

public slots:
    void setTarget(QQuickItem *target);
    void setLocation(QString location);
    void setLoadMode(LoadMode loadMode);
    void setAsynchronous(bool asynchronous);

private slots:
    void loadIfPossible();
    void reparentItemToTarget(QQuickItem *item);
    void reparentItemsToTarget(QList<QQuickItem *> items);
    QQuickItem *createItemFromUrl(const QString &itemUrl);
    void createItemAsync(const QString &itemUrl);
    void asyncItemReady();
    void handleIncubatorError();

private:
    LoadMode m_loadMode;
    QPointer<QQuickItem> m_target;
    QString m_location;
    QList<QQuickItem *> m_defaultItems;
    QString m_pluginId;
    bool m_asynchronous;
    IncubatorList m_incubators;
};

#endif // ITEMREGISTRY_H
