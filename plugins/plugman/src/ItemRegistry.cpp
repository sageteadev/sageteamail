#include "ItemRegistry.h"
#include <QMetaObject>
#include <QQuickWindow>
#include "PluginRegistry.h"

Q_LOGGING_CATEGORY(PLUGIN_ITEM_REGISTRY, "sageteamail.plugman.itemregistry")

ItemRegistry::ItemRegistry(QObject *parent) : QObject(parent),
    m_loadMode(LoadAll), m_asynchronous(true)
{
}

QQuickItem* ItemRegistry::target() const
{
    return m_target;
}

QString ItemRegistry::location() const
{
    return m_location;
}

QQmlListProperty<QQuickItem> ItemRegistry::defaultItems()
{
    return QQmlListProperty<QQuickItem>(this, m_defaultItems);
}

ItemRegistry::LoadMode ItemRegistry::loadMode() const
{
    return m_loadMode;
}

bool ItemRegistry::asynchronous() const
{
    return m_asynchronous;
}

QString ItemRegistry::findFirstEnabled(const QString &location)
{
    if (location.isEmpty()) {
        return QString();
    }
    auto plugins = PluginRegistry::instance()->getByLocation(location);
    if (plugins.isEmpty()) {
        return QString();
    }
    qCDebug(PLUGIN_ITEM_REGISTRY) << "Found first enabled plugin";
    auto firstplugin = qobject_cast<SageteaMailPlugin *>(plugins.first());
    return firstplugin->component();
}

void ItemRegistry::setTarget(QQuickItem *target)
{
    if (m_target == target)
        return;

    m_target = target;
    emit targetChanged(target);
    loadIfPossible();
}

void ItemRegistry::setLocation(QString location)
{
    if (m_location == location)
        return;

    m_location = location;
    emit locationChanged(location);
    loadIfPossible();
}

void ItemRegistry::setLoadMode(ItemRegistry::LoadMode loadMode)
{
    if (m_loadMode == loadMode)
        return;

    m_loadMode = loadMode;
    emit loadModeChanged(loadMode);
}

void ItemRegistry::setAsynchronous(bool asynchronous)
{
    if (m_asynchronous == asynchronous)
        return;

    m_asynchronous = asynchronous;
    emit asyncChanged(asynchronous);
}

void ItemRegistry::loadIfPossible()
{
    if (m_location.isEmpty() || m_target.isNull()) {
        return;
    }

    auto plugins = PluginRegistry::instance()->getByLocation(m_location);

    switch(m_loadMode) {
    case DefaultOnly:
    {
        qCDebug(PLUGIN_ITEM_REGISTRY) << "Loading default items only";
        reparentItemsToTarget(m_defaultItems);
        break;
    }
    case LoadFirstEnabled:
    {
        if (plugins.isEmpty()) {
            setLoadMode(DefaultOnly);
            loadIfPossible();
            return;
        }
        qCDebug(PLUGIN_ITEM_REGISTRY) << "Loading first enabled plugin";
        auto firstplugin = qobject_cast<SageteaMailPlugin *>(plugins.first());
        if (m_asynchronous) {
            createItemAsync(firstplugin->component());
        } else {
            QQuickItem *firstItem = createItemFromUrl(firstplugin->component());
            reparentItemToTarget(firstItem);
        }
        break;
    }
    case LoadLastEnabled:
    {
        if (plugins.isEmpty()) {
            setLoadMode(DefaultOnly);
            loadIfPossible();
            return;
        }
        qCDebug(PLUGIN_ITEM_REGISTRY) << "Loading last enabled plugin";
        auto lastplugin = qobject_cast<SageteaMailPlugin *>(plugins.last());
        if (m_asynchronous) {
            createItemAsync(lastplugin->component());
        } else {
            QQuickItem *lastItem = createItemFromUrl(lastplugin->component());
            reparentItemToTarget(lastItem);
        }
        break;
    }
    case LoadAll:
    {
        qCDebug(PLUGIN_ITEM_REGISTRY) << "Loading all plugins and default items";
        reparentItemsToTarget(m_defaultItems); // default items get appended first
        for (auto plugin : plugins) {
            if (auto dp = qobject_cast<SageteaMailPlugin *>(plugin)) {
                if (m_asynchronous) {
                    createItemAsync(dp->component());
                } else {
                    QQuickItem *item = createItemFromUrl(dp->component());
                    reparentItemToTarget(item);
                }
            }
        }
        break;
    }
    case LoadById:
    {
        if (plugins.isEmpty()) {
            setLoadMode(DefaultOnly);
            loadIfPossible();
            return;
        }
        qCDebug(PLUGIN_ITEM_REGISTRY) << "Looking for plugin with id: " << m_pluginId;
        QQuickItem *item = Q_NULLPTR;
        for (auto plugin : plugins) {
            if (plugin->pluginId() == m_pluginId) {
                if (auto dp = qobject_cast<SageteaMailPlugin *>(plugin)) {
                    qCDebug(PLUGIN_ITEM_REGISTRY) << "Found plugin with id: " << m_pluginId;
                    item = createItemFromUrl(dp->component());
                }
            }
        }
        if (item) {
            reparentItemToTarget(item);
        } else {
            qCDebug(PLUGIN_ITEM_REGISTRY) << "No plugin found with id: " << m_pluginId;
            qCDebug(PLUGIN_ITEM_REGISTRY) << "Falling back to default items";
            reparentItemsToTarget(m_defaultItems);
        }
        break;
    }
    }
}

void ItemRegistry::reparentItemToTarget(QQuickItem *item)
{
    if (item) {
        item->setParentItem(m_target);
        item->setVisible(true);
    } else {
        qCWarning(PLUGIN_ITEM_REGISTRY) << "Invalid item";
    }
}

void ItemRegistry::reparentItemsToTarget(QList<QQuickItem *> items)
{
    for (auto item : items) {
        reparentItemToTarget(item);
    }
}

QQuickItem *ItemRegistry::createItemFromUrl(const QString &itemUrl)
{
    if (itemUrl.isEmpty()) {
        qCWarning(PLUGIN_ITEM_REGISTRY) << "Invalid component url";
        return Q_NULLPTR;
    }
    auto engine = qmlEngine(this);
    QQmlComponent itemComponent(engine, QUrl::fromLocalFile(itemUrl), m_target);
    if (itemComponent.isError()) {
        for (auto error : itemComponent.errors()) {
            qCDebug(PLUGIN_ITEM_REGISTRY) << "Failed loading plugin with error:";
            qCDebug(PLUGIN_ITEM_REGISTRY) << error.toString();
        }
        return Q_NULLPTR;
    }

    return qobject_cast<QQuickItem *>(itemComponent.create(engine->contextForObject(this)));
}

void ItemRegistry::createItemAsync(const QString &itemUrl)
{
    if (itemUrl.isEmpty()) {
        qCWarning(PLUGIN_ITEM_REGISTRY) << "Invalid component url";
        return;
    }
    PluginIncubator *incubator = new PluginIncubator(this);
    connect(incubator, &PluginIncubator::objectReady, this, &ItemRegistry::asyncItemReady);
    connect(incubator, &PluginIncubator::error, this, &ItemRegistry::handleIncubatorError);
    incubator->setSourceUrl(qmlEngine(this), QUrl::fromLocalFile(itemUrl));
    m_incubators << incubator;
}

void ItemRegistry::asyncItemReady()
{
    PluginIncubator *incubator = qobject_cast<PluginIncubator *>(sender());
    if (incubator->status() == QQmlIncubator::Ready) {
        QObject *itemObject = incubator->object();
        // We also support loading a QQUickWindow and showing it straight away
        // if the base component is a Window{} component.
        if (itemObject->metaObject()->className() == QByteArrayLiteral("QQuickWindowQmlImpl")) {
            QQuickWindow *window = qobject_cast<QQuickWindow *>(itemObject);
            window->show();
        } else {
            QQuickItem *item = qobject_cast<QQuickItem *>(itemObject);
            if (item) {
                reparentItemToTarget(item);
            } else {
                qCWarning(PLUGIN_ITEM_REGISTRY) << "Failed casting plugin to qquickitem";
                incubator->deleteLater();
            }
        }
    } else {
        incubator->deleteLater();
    }
}

void ItemRegistry::handleIncubatorError()
{

}

