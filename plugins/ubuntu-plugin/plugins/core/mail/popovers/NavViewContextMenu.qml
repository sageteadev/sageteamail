/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>

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
import QtQuick 2.4
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import SageteaMail.Components 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Lomiri.Components 1.0

Popover {
    id: actionPopover
    property var folder

    contentWidth: units.gu(25)

    Column {
        id: containerLayout
        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }

        ContextGroup {
            contextActions: [
                ContextAction {
                    visible: folder.supportedActions & MessageSet.SyncAction
                    description: qsTr("Sync folder")
                    actionIcon: Icons.SyncIcon
                    onTriggered: {
                        MailboxActions.syncStandardFolders(folder.folderType)
                        PopupUtils.close(actionPopover)
                    }
                },
                ContextAction {
                    visible: folder.supportedActions & MessageSet.ExportPending
                    description: qsTr("Send pending")
                    actionIcon: Icons.SendIcon
                    onTriggered: {
                        MessageActions.sendPendingMessages()
                        PopupUtils.close(actionPopover)
                    }
                },
                ContextAction {
                    visible: folder.supportedActions & MessageSet.MarkAsRead
                    description: qsTr("Mark folder read")
                    actionIcon: Icons.MailReadIcon
                    onTriggered: {
                        MailboxActions.markStandardFoldersRead(folder.folderType)
                        PopupUtils.close(actionPopover)
                    }
                }
            ]
        }

        ContextGroup {
            contextActions: [
                ContextAction {
                    visible: folder.supportedActions & MessageSet.EmptyTrash
                    description: qsTr("Empty trash")
                    actionIcon: Icons.DeleteIcon
                    onTriggered: {
                        MailboxActions.emptyTrash()
                        PopupUtils.close(actionPopover)
                    }
                }
            ]
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width * 4 / 5
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            text: i18n.tr("You are runnig a free edition of SageTea Mail, condider upgrade to <a href=\"https://sagetea.ai/product/sagetea-mail/\">Pro</a>")
            font.bold: true
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }
}
