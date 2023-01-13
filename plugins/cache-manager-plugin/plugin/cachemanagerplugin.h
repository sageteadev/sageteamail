#ifndef CACHEMANAGERPLUGIN_H
#define CACHEMANAGERPLUGIN_H

#include <QObject>
#include <QDBusPendingCallWatcher>
#include <PluginInfo.h>
#include <PluginLoader.h>
#include <QScopedPointer>
#include <qmailmessagekey.h>
#include <qmailmessage.h>

class CacheManagerService : public ServicePlugin
{
    Q_OBJECT
public:
    explicit CacheManagerService(QObject *parent = 0);

    enum Period {
        WEEK = 0,
        FORTNIGHT = 1,
        MONTH = 2,
        THREE_MONTH = 3,
        SIX_MONTH = 4,
        ALL = 5
    };

    // PluginInfo interface
public:
    virtual QString pluginId() const override;
    virtual QString location() const override;
    virtual QString i18n() const override;
    virtual QVariantMap documentation() const override;

    // ServicePlugin interface
public:
    virtual void start();
    virtual void stop();

    virtual void messageReceived(const QString &key, const QVariant &value);

protected:

    void buildQuery(const QMailAccountId &id, const Period &period);

    void queryMessages(const QMailMessageKey &key);

private slots:
    void sendQueries();
    void removePending();
    void handleQueryResponse(QDBusPendingCallWatcher *call);

private:
    QList<QMailMessageKey> m_queries;
    QMailMessageIdList m_toRemove;
};

class CacheManagerPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "org.sagetea.cache-manager")

    // PluginInterface interface
public:
    virtual QString name() const;
    virtual QString description() const;
    virtual PluginInfo *create(QObject *parent) const;
};


#endif // CACHEMANAGERPLUGIN_H
