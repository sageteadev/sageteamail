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
#ifndef NOTIFICATIONWATCHER_H
#define NOTIFICATIONWATCHER_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QJsonObject>
#include <qmailmessage.h>
#include <Accounts.h>

class NotificationWatcher : public QObject
{
    Q_OBJECT
public:
    explicit NotificationWatcher(QObject *parent = 0);

    enum NotificationType {
        NONE,
        INBOX_ONLY,
        CONTACTS_ONLY,
        EVERYTHING
    };

signals:
    void newMessage(const QMailAccountId &account, const QJsonObject &message);

public slots:
    void handleNewMessages(const QMailMessageIdList &msgList);
    void handleUpdatedMessages(const QMailMessageIdList &msgList);
    void handleRemovedMessages(const QMailMessageIdList &msgList);

private:
    QJsonObject buildMessage(const QMailMessage &msg);
    QJsonObject card(const QMailMessage &msg);
    QJsonObject vibrate();
    QJsonObject emblemCounter();
    bool notifyEnabled(const QMailMessageMetaData &meta);
    NotificationType notificationType(const QMailMessageMetaData &meta);
    bool canPostNotification(const QMailMessageMetaData &meta);
    Account *getAccount(const quint64 &id);
    QString getTag(const QMailMessageId &id);

private:
    QDateTime m_started;
    QScopedPointer<Accounts> m_accounts;
};

#endif // NOTIFICATIONWATCHER_H
