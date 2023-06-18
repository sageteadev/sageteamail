#include "cachemanagerplugin.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>
#include <QDebug>
#include <SnapStandardPaths.h>
#include <MailServiceClient.h>

#define CACHE_MGR_NS "cache-mgr"
#define CACHE_MGR_PERIOD "period"

namespace {
    inline QMailMessageKey to_msg_key(const QByteArray &key) {
        QMailMessageKey mKey;
        QByteArray m = key;
        QDataStream keystream(&m, QIODevice::ReadWrite);
        mKey.deserialize(keystream);
        return mKey;
    }

    inline QByteArray msg_key_bytes(const QMailMessageKey &key) {
        QByteArray msgKey;
        QDataStream keystream(&msgKey, QIODevice::WriteOnly);

        key.serialize(keystream);
        return msgKey;
    }

    inline QMailMessageSortKey to_msg_sort_key(const QByteArray &key) {
        QMailMessageSortKey sKey;
        QByteArray s = key;
        QDataStream skeystream(&s, QIODevice::ReadWrite);
        sKey.deserialize(skeystream);
        return sKey;
    }

    inline QByteArray msg_sort_key_bytes(const QMailMessageSortKey &key) {
        QByteArray sortKey;
        QDataStream keystream(&sortKey, QIODevice::WriteOnly);

        key.serialize(keystream);

        return sortKey;
    }

    inline QMailMessageIdList from_dbus_msglist(const QList<quint64> &ids) {
        QMailMessageIdList list;
        foreach(const quint64 &id, ids) {
            QMailMessageId msgId(id);
            list << msgId;
        }
        return list;
    }

    inline QList<quint64> to_dbus_msglist(const QMailMessageIdList &ids) {
        QList<quint64> list;
        foreach(const QMailMessageId &id, ids) {
            list << id.toULongLong();
        }
        return list;
    }
}

CacheManagerService::CacheManagerService(QObject *parent): ServicePlugin(parent)
{
}

QString CacheManagerService::pluginId() const
{
    return QStringLiteral("cache-manager");
}

QString CacheManagerService::location() const
{
    return QStringLiteral("SageteaMail::Service");
}

QString CacheManagerService::i18n() const
{
    return QString();
}

void CacheManagerService::start()
{
    qDebug() << "[CacheManagerService] >> Ready";
}

void CacheManagerService::stop()
{
}

void CacheManagerService::messageReceived(const QString &key, const QVariant &value)
{
    if (key.startsWith("prune:")) {
        QString id = key.split(':').last();
        quint64 aId = id.toUInt();
        QMailAccountId account(aId);
        if (!account.isValid()) {
            qDebug() << "[CacheManagerService] >> Invalid account id.";
            return;
        }
        int p = value.toInt();
        if (p < WEEK && p > ALL) {
            qDebug() << "[CacheManagerService] >> Invalid period";
            return;
        }
        Period period = static_cast<Period>(p);
        buildQuery(account, period);
    }
}

void CacheManagerService::buildQuery(const QMailAccountId &id, const CacheManagerService::Period &period)
{
    QMailMessageKey periodKey;

    QDateTime now = QDateTime::currentDateTime();

    switch (static_cast<Period>(period)) {
    case WEEK:
    {
        qDebug() << "[CacheManagerService] >> Querying for messages older than 1 week";
        QDateTime week = now.addDays(-7);
        periodKey = QMailMessageKey::timeStamp(week, QMailDataComparator::LessThan);
        break;
    }
    case FORTNIGHT:
    {
        qDebug() << "[CacheManagerService] >> Querying for messages older than 2 weeks";
        QDateTime fortnight = now.addDays(-14);
        periodKey = QMailMessageKey::timeStamp(fortnight, QMailDataComparator::LessThan);
        break;
    }
    case MONTH:
    {
        qDebug() << "[CacheManagerService] >> Querying for messages older than 1 month";
        QDateTime month = now.addMonths(-1);
        periodKey = QMailMessageKey::timeStamp(month, QMailDataComparator::LessThan);
        break;
    }
    case THREE_MONTH:
    {
        qDebug() << "[CacheManagerService] >> Querying for messages older than 3 months";
        QDateTime month = now.addMonths(-3);
        periodKey = QMailMessageKey::timeStamp(month, QMailDataComparator::LessThan);
        break;
    }
    case SIX_MONTH:
    {
        qDebug() << "[CacheManagerService] >> Querying for messages older than 6 months";
        QDateTime month = now.addMonths(-6);
        periodKey = QMailMessageKey::timeStamp(month, QMailDataComparator::LessThan);
        break;
    }
    case ALL:
        return; // do nothing as we don't want to nuqe it all for now
    }

    QMailMessageKey query = QMailMessageKey::parentAccountId(id) & periodKey;
    m_queries << query;
    if (!m_queries.isEmpty())
        QTimer::singleShot(1000, this, &CacheManagerService::sendQueries);
}

void CacheManagerService::sendQueries()
{
    qDebug() << "[CacheManagerService::sendQueries] >> Sending queries";
    foreach(const QMailMessageKey &key, m_queries) {
        queryMessages(key);
        m_queries.removeAll(key);
    }
}

void CacheManagerService::removePending()
{
    qDebug() << "[CacheManagerService::removePending] >> Removing " << m_toRemove.count() << "messages from cache";
    Client::instance()->pruneCache(m_toRemove);
    m_toRemove.clear();
}

void CacheManagerService::queryMessages(const QMailMessageKey &key)
{
    QMailMessageSortKey sortKey;
    QDBusPendingReply<QList<quint64> > reply = Client::instance()->bus()->queryMessages(
                msg_key_bytes(key),
                msg_sort_key_bytes(sortKey),
                0);

    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(reply, this);

    connect(watcher, &QDBusPendingCallWatcher::finished, this, &CacheManagerService::handleQueryResponse);
}

void CacheManagerService::handleQueryResponse(QDBusPendingCallWatcher *call)
{
    qDebug() << "[CacheManagerService::handleQueryResponse] >> handling response";
    QDBusPendingReply<QList<quint64> > reply = *call;
    if (reply.isError()) {
        qDebug() << "Reply error for refresh response";
        return;
    }
    QList<quint64> ids = reply.argumentAt<0>();
    m_toRemove << from_dbus_msglist(ids);
    QTimer::singleShot(1000, this, &CacheManagerService::removePending);
    call->deleteLater();
}

QVariantMap CacheManagerService::documentation() const
{
    return QVariantMap();
}

QString CacheManagerPlugin::name() const
{
    return QStringLiteral("cache-manager");
}

QString CacheManagerPlugin::description() const
{
    return QStringLiteral("SageteaMail cache manager");
}

PluginInfo *CacheManagerPlugin::create(QObject *parent) const
{
    return new CacheManagerService(parent);
}
