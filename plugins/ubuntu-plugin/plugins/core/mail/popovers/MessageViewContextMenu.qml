/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>
   Copyright (C) 2022 - 2023 Rúben Carneiro <rubencarneiro01@gmail.com>

   This file is part of SageTea Mail, email client for Ubuntu devices

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
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.3
import SageteaMail.Components 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Ubuntu.Components 1.0

Popover {
    id: actionPopover
    z: 100 // Prevent being rendered below the message header
    property var msg
    property var ctxtModel

    Column {
        id: containerLayout

        anchors {
            left: parent.left
            top: parent.top
            right: parent.right
        }

        ContextGroup {
            // only show this group on links
            visible: ctxtModel && ctxtModel.linkUrl.toString()
            contextActions: [
                ContextAction {
                    description: qsTr("Open in browser")
                    actionIcon: Icons.BrowserIcon
                    onTriggered: {
                        Qt.openUrlExternally(ctxtModel.linkUrl.toString())
                        PopupUtils.close(actionPopover)
                    }
                },
                ContextAction {
                    description: qsTr("Copy link")
                    actionIcon: Icons.CopyIcon
                    onTriggered: {
                        Clipboard.push(["text/plain", ctxtModel.linkUrl.toString()])
                        PopupUtils.close(actionPopover)
                    }
                },
                ContextAction {
                    visible: isRunningOnMir
                    description: qsTr("Share link")
                    actionIcon: Icons.ShareIcon
                    onTriggered: {
                        PopupActions.showNotice(PopupKeys.popupMainWindow, "not implemented yet. Fix it before release!!!!")
                        PopupUtils.close(actionPopover)
                    }
                }

            ]
        }

        ContextGroup {
            contextActions: [
                ContextAction {
                    description: qsTr("Reply")
                    actionIcon: Icons.MailRepliedAllIcon
                    onTriggered: {
                        ComposerActions.respondToMessage(msg.isListPost ? SubmissionManager.ReplyList : SubmissionManager.Reply, msg.messageId)
                        PopupUtils.close(actionPopover)
                    }
                },

                ContextAction {
                    description: qsTr("Reply all")
                    actionIcon: Icons.MailRepliedIcon
                    onTriggered: {
                        ComposerActions.respondToMessage(SubmissionManager.ReplyAll, msg.messageId)
                        PopupUtils.close(actionPopover)
                    }
                },
                ContextAction {
                    description: qsTr("Forward")
                    actionIcon: Icons.MailForwardedIcon
                    onTriggered: {
                        ComposerActions.forwardMessage(SubmissionManager.ForwardInline, msg.messageId)
                        PopupUtils.close(actionPopover)
                    }
                }
            ]
        }
        ContextGroup {
            contextActions: [
                ContextAction {
                    description: qsTr("View source")
                    actionIcon: Icons.NoteIcon
                    onTriggered: {
                        PopupActions.showNotice(PopupKeys.popupMainWindow, "not implemented yet. Fix it before release!!!!")
                        PopupUtils.close(actionPopover)
                    }
                }
            ]
        }
    }
}

