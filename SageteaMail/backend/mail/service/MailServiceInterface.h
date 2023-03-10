/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp /home/dan/dekko/Dekko/backend/mail/service/dbus/service_worker.xml -N -p MailServiceInterface -c MailServiceInterface
 *
 * qdbusxml2cpp is Copyright (C) 2016 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef MAILSERVICEINTERFACE_H
#define MAILSERVICEINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.sagetea.MailService
 */
class MailServiceInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.sagetea.MailService"; }

public:
    MailServiceInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~MailServiceInterface();

    Q_PROPERTY(bool hasUndoableAction READ hasUndoableAction)
    inline bool hasUndoableAction() const
    { return qvariant_cast< bool >(property("hasUndoableAction")); }

    Q_PROPERTY(QString undoDescription READ undoDescription)
    inline QString undoDescription() const
    { return qvariant_cast< QString >(property("undoDescription")); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> createStandardFolders(qulonglong accountId)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(accountId);
        return asyncCallWithArgumentList(QStringLiteral("createStandardFolders"), argumentList);
    }

    inline QDBusPendingReply<> deleteMessages(const QList<quint64> &ids)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(ids);
        return asyncCallWithArgumentList(QStringLiteral("deleteMessages"), argumentList);
    }

    inline QDBusPendingReply<> downloadMessagePart(qulonglong msgId, const QString &partLocation)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgId) << QVariant::fromValue(partLocation);
        return asyncCallWithArgumentList(QStringLiteral("downloadMessagePart"), argumentList);
    }

    inline QDBusPendingReply<> downloadMessages(const QList<quint64> &msgIds)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds);
        return asyncCallWithArgumentList(QStringLiteral("downloadMessages"), argumentList);
    }

    inline QDBusPendingReply<> emptyTrash(const QList<quint64> &accountIds)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(accountIds);
        return asyncCallWithArgumentList(QStringLiteral("emptyTrash"), argumentList);
    }

    inline QDBusPendingReply<> markFolderRead(qulonglong folderId)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(folderId);
        return asyncCallWithArgumentList(QStringLiteral("markFolderRead"), argumentList);
    }

    inline QDBusPendingReply<> markMessageForwarded(const QList<quint64> &msgIds)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds);
        return asyncCallWithArgumentList(QStringLiteral("markMessageForwarded"), argumentList);
    }

    inline QDBusPendingReply<> markMessagesDone(const QList<quint64> &msgIds, bool done)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(done);
        return asyncCallWithArgumentList(QStringLiteral("markMessagesDone"), argumentList);
    }

    inline QDBusPendingReply<> markMessagesImportant(const QList<quint64> &msgIds, bool important)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(important);
        return asyncCallWithArgumentList(QStringLiteral("markMessagesImportant"), argumentList);
    }

    inline QDBusPendingReply<> markMessagesRead(const QList<quint64> &msgIds, bool read)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(read);
        return asyncCallWithArgumentList(QStringLiteral("markMessagesRead"), argumentList);
    }

    inline QDBusPendingReply<> markMessagesReplied(const QList<quint64> &msgIds, bool all)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(all);
        return asyncCallWithArgumentList(QStringLiteral("markMessagesReplied"), argumentList);
    }

    inline QDBusPendingReply<> markMessagesTodo(const QList<quint64> &msgIds, bool read)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(read);
        return asyncCallWithArgumentList(QStringLiteral("markMessagesTodo"), argumentList);
    }

    inline QDBusPendingReply<> moveToFolder(const QList<quint64> &msgIds, qulonglong folderId)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(folderId);
        return asyncCallWithArgumentList(QStringLiteral("moveToFolder"), argumentList);
    }

    inline QDBusPendingReply<> moveToStandardFolder(const QList<quint64> &msgIds, int folderType, bool userTriggered)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds) << QVariant::fromValue(folderType) << QVariant::fromValue(userTriggered);
        return asyncCallWithArgumentList(QStringLiteral("moveToStandardFolder"), argumentList);
    }

    inline QDBusPendingReply<> pruneCache(const QList<quint64> &msgIds)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgIds);
        return asyncCallWithArgumentList(QStringLiteral("pruneCache"), argumentList);
    }

    inline QDBusPendingReply<QList<quint64> > queryFolders(const QByteArray &folderKey, const QByteArray &sortKey, int limit)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(folderKey) << QVariant::fromValue(sortKey) << QVariant::fromValue(limit);
        return asyncCallWithArgumentList(QStringLiteral("queryFolders"), argumentList);
    }

    inline QDBusPendingReply<QList<quint64> > queryMessages(const QByteArray &msgKey, const QByteArray &sortKey, int limit)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgKey) << QVariant::fromValue(sortKey) << QVariant::fromValue(limit);
        return asyncCallWithArgumentList(QStringLiteral("queryMessages"), argumentList);
    }

    inline QDBusPendingReply<> removeMessage(qulonglong msgId, int option)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgId) << QVariant::fromValue(option);
        return asyncCallWithArgumentList(QStringLiteral("removeMessage"), argumentList);
    }

    inline QDBusPendingReply<> restoreMessage(qulonglong id)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(id);
        return asyncCallWithArgumentList(QStringLiteral("restoreMessage"), argumentList);
    }

    inline QDBusPendingReply<> sendAnyQueuedMail()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("sendAnyQueuedMail"), argumentList);
    }

    inline QDBusPendingReply<> sendMessage(qulonglong msgId)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgId);
        return asyncCallWithArgumentList(QStringLiteral("sendMessage"), argumentList);
    }

    inline QDBusPendingReply<> sendPendingMessages()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("sendPendingMessages"), argumentList);
    }

    inline QDBusPendingReply<> syncFolders(qulonglong accountId, const QList<quint64> &folders)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(accountId) << QVariant::fromValue(folders);
        return asyncCallWithArgumentList(QStringLiteral("syncFolders"), argumentList);
    }

    inline QDBusPendingReply<> synchronizeAccount(qulonglong accountId)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(accountId);
        return asyncCallWithArgumentList(QStringLiteral("synchronizeAccount"), argumentList);
    }

    inline QDBusPendingReply<int> totalCount(const QByteArray &msgKey)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(msgKey);
        return asyncCallWithArgumentList(QStringLiteral("totalCount"), argumentList);
    }

    inline QDBusPendingReply<> undoActions()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("undoActions"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void accountSynced(qulonglong id);
    void actionFailed(qulonglong id, int statusCode, const QString &statusText);
    void clientError(qulonglong accountId, int error, const QString &errorString);
    void messageFetchFailed(const QList<quint64> &msgIds);
    void messagePartFetchFailed(qulonglong msgId, const QString &partLocation);
    void messagePartNowAvailable(qulonglong msgId, const QString &partLocation);
    void messageRestored(qulonglong msgId);
    void messageSendingFailed(const QList<quint64> &msgIds, int error);
    void messagesNowAvailable(const QList<quint64> &msgIds);
    void messagesSent(const QList<quint64> &msgIds);
    void queueChanged();
    void standardFoldersCreated(qulonglong accountId, bool created);
    void syncAccountFailed(qulonglong id);
    void undoCountChanged();
    void updatesRolledBack();
    void foldersSynced(const qulonglong accountId, const QList<quint64> &folderIds);
    void foldersSyncFailed(const qulonglong accountId, const QList<quint64> &folderIds);
};

#endif
