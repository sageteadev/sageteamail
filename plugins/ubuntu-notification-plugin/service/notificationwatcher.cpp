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
#include "notificationwatcher.h"
#include <QJsonArray>
#include <qmailstore.h>
#include "pushclient.h"

#define CFG_NAMESPACE "sageteamail-notify"
#define NOTIFY_ENABLED "enabled"
#define NOTIFY_TYPE "type"

NotificationWatcher::NotificationWatcher(QObject *parent) : QObject(parent),
    m_started(QDateTime::currentDateTime()),
    m_accounts(new Accounts)
{
    m_accounts->setFilter(Accounts::CanReceive);
    connect(QMailStore::instance(), SIGNAL(messagesAdded(QMailMessageIdList)), this, SLOT(handleNewMessages(QMailMessageIdList)));
    connect(QMailStore::instance(), SIGNAL(messagesUpdated(QMailMessageIdList)), this, SLOT(handleUpdatedMessages(QMailMessageIdList)));
    connect(QMailStore::instance(), SIGNAL(messagesRemoved(QMailMessageIdList)), this, SLOT(handleRemovedMessages(QMailMessageIdList)));
}

void NotificationWatcher::handleNewMessages(const QMailMessageIdList &msgList)
{
    Q_FOREACH(const QMailMessageId &id, msgList) {
        QMailMessageMetaData d(id);
        if (notifyEnabled(d) && canPostNotification(d)) {
            QMailMessage msg(id);
            auto message = buildMessage(msg);
            PushClient::instance()->send(message);
        }
    }
}

void NotificationWatcher::handleUpdatedMessages(const QMailMessageIdList &msgList)
{
    Q_FOREACH(const QMailMessageId &id, msgList) {
        QMailMessageMetaData d(id);
        const QString tag = getTag(id);
        if (PushClient::instance()->hasTag(tag)) {
            if ((d.status() & QMailMessage::Read) || (d.status() & QMailMessage::ReadElsewhere)) {
                PushClient::instance()->clearPersistent(tag);
            }
            // We should also check if the message is still new but we have now received the preview
            // then we can update the notification with the new content.
            // Probably requires a remove & re-post but with no popup
            if (notifyEnabled(d) && canPostNotification(d)) {
                // TODO!!
            }
        }
    }
}

void NotificationWatcher::handleRemovedMessages(const QMailMessageIdList &msgList)
{
    Q_FOREACH(const QMailMessageId &id, msgList) {
        const QString tag = getTag(id);
        if (PushClient::instance()->hasTag(tag)) {
            PushClient::instance()->clearPersistent(tag);
        }
    }
}

QJsonObject NotificationWatcher::buildMessage(const QMailMessage &msg)
{
    QJsonObject notification;
    notification["tag"] = getTag(msg.id());
    notification["card"] = card(msg);
    notification["sound"] = true;
    notification["vibrate"] = vibrate();
//    notification["emblem-counter"] = emblemCounter();
    QJsonObject message;
    message["notification"] = notification;
    return message;
}

QJsonObject NotificationWatcher::card(const QMailMessage &msg)
{
    QJsonObject c;
    c["summary"] = msg.from().name();
    c["body"] = msg.subject() + "\n" + msg.preview();
    c["popup"] = true;
    c["persist"] = true;
    Q_UNUSED(msg);
//    c["timestamp"] = timestamp;
    QJsonArray actions = QJsonArray();
    const QString pId = QString::number(msg.parentAccountId().toULongLong());
    const QString fId = QString::number(msg.parentFolderId().toULongLong());
    const QString mId = QString::number(msg.id().toULongLong());
//    QString actionUri = QString("sageteamail://notify/%1/%2/%3").arg(pId, fId, mId);
    QString actionUri = QStringLiteral("appid://sageteamail2.sagetea/sageteamail/current-user-version");
    actions.append(actionUri);
    c["actions"] = actions;
    return c;
}

QJsonObject NotificationWatcher::vibrate()
{
    QJsonObject v;
    v["pattern"] = QJsonArray() << 200 << 100;
    v["duration"] = 200;
    v["repeat"] = 2;
    return v;
}

QJsonObject NotificationWatcher::emblemCounter()
{
    QJsonObject e;
    e["count"] = 5;
    e["visible"] = true;
    return e;
}

bool NotificationWatcher::notifyEnabled(const QMailMessageMetaData &meta)
{
    const quint64 id = meta.parentAccountId().toULongLong();
    Account *account = getAccount(id);
    if (account) {
        const QString &value = account->customValue(CFG_NAMESPACE, NOTIFY_ENABLED);
        if (!value.isEmpty()) {
            return value.toInt() > 0;
        }
        account->setCustomValue(CFG_NAMESPACE, NOTIFY_ENABLED, QString::number(1));
        return true;
    }
    return false;
}

NotificationWatcher::NotificationType NotificationWatcher::notificationType(const QMailMessageMetaData &meta)
{
    const quint64 id = meta.parentAccountId().toULongLong();
    Account *account = getAccount(id);
    if (account) {
        const QString &value = account->customValue(CFG_NAMESPACE, NOTIFY_TYPE);
        if (!value.isEmpty()) {
            int type = value.toInt();
            if (type < 0 || type > 3) {
                return NONE;
            }
            return (NotificationType)type;
        }
        account->setCustomValue(CFG_NAMESPACE, NOTIFY_TYPE, QString::number(INBOX_ONLY));
        return INBOX_ONLY;
    }
    return NONE;
}

bool NotificationWatcher::canPostNotification(const QMailMessageMetaData &meta)
{
    const quint64 id = meta.parentAccountId().toULongLong();
    Account *account = getAccount(id);
    bool result = false;
    if (!account)
        return result;

    if (meta.date().toLocalTime() < m_started) {
        return result;
    }

    if ((meta.status() & QMailMessage::Incoming) && (meta.status() & QMailMessage::New)) {

        switch (notificationType(meta)) {
        case NotificationType::INBOX_ONLY:
        {
            QMailFolder folder(meta.parentFolderId());
            QMailAccount account(folder.parentAccountId());
            if (account.standardFolders().values().contains(folder.id())) {
                QMailFolder::StandardFolder standardFolder = account.standardFolders().key(folder.id());
                result = standardFolder == QMailFolder::InboxFolder;
            }
            break;
        }
        case NotificationType::CONTACTS_ONLY:
        case NotificationType::EVERYTHING:
        {
            result = true;
            break;
        }
        case NotificationType::NONE:
            break;
        }
    }

    return result;
}

Account *NotificationWatcher::getAccount(const quint64 &id)
{
    if (m_accounts->hasAccount(id)) {
        return m_accounts->get(id);
    }
    return Q_NULLPTR;
}

QString NotificationWatcher::getTag(const QMailMessageId &id)
{
    return QString("sageteamail-%1").arg(QString::number(id.toULongLong()));
}


