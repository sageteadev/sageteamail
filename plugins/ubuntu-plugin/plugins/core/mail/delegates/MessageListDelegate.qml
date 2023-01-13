/* Copyright (C) 2014-2016 Dan Chapman <dpniel@ubuntu.com>

   This file is part of the Dekko email client for Ubuntu Touch/

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
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3
import Ubuntu.Components.Popups 1.1
import QtGraphicalEffects 1.0
import SageteaMail.Components 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Settings 1.0
import SageteaMail.Ubuntu.Components 1.0
import "../popovers"
import SageteaMail.Ubuntu.Constants 1.0

ListItemWithActions {
    id: normalMessageItemDelegate

    //---------------------------------
    // PROPERTIES
    //---------------------------------
    property bool canLoad: false
    property var msg
    onMsgChanged: {
        if (msg) {
            canLoad = true
        } else {
            canLoad = false
        }
    }

    property alias avatarVisible: inner_name.visible

    property Action flagAction: Action {
        id: flagAction
        text: canLoad && msg.isImportant ? qsTr("Un-mark flagged") : qsTr("Mark flagged")
        iconSource: canLoad && msg.isImportant ? Paths.actionIconUrl(Paths.UnStarredIcon) :
                                             Paths.actionIconUrl(Paths.StarredIcon)

        onTriggered: MessageActions.markMessageImportant(msg.messageId, !msg.isImportant)
    }

    property Action readAction: Action {
        id: markReadAction
        text: canLoad && msg.isRead ? qsTr("Mark as un-read") : qsTr("Mark as read")
        iconSource: canLoad && msg.isRead ? Paths.actionIconUrl(Paths.MailUnreadIcon) :
                                        Paths.actionIconUrl(Paths.MailReadIcon)
        onTriggered: Client.markMessageRead(msg.messageId, !msg.isRead);
    }

    property Action moveAction: Action {
        text: qsTr("Move message")
        iconSource: Paths.actionIconUrl(Paths.InboxIcon)
    }

    property Action contextAction: Action {
        text: qsTr("Context menu")
        iconName: "contextual-menu"
        onTriggered: rightClickActions.trigger()
    }

    property Action rightClickActions: Action {
        id: rightClick
        onTriggered: PopupUtils.open(Qt.resolvedUrl("../popovers/MessageListActionPopover.qml"),
                                     msgListDelegate,
                                     {
                                         msg: normalMessageItemDelegate.msg
                                     })
    }

    //---------------------------------
    // OBJECT PROPERTIES
    //---------------------------------
    showDivider: false
    height: main.height
    triggerActionOnMouseRelease: true

    //---------------------------------
    // COMPONENTS
    //---------------------------------
    ListItemLayout {
        id: main
        title.text: msg ? msg.from.name : ""
        title.font.bold: canLoad && !msg.isRead
        title.fontSize: "x-small"
        title.elide: Text.ElideRight
        title.wrapMode: Text.NoWrap

        subtitle.text: msg ? msg.subject : ""
        subtitle.font.bold: canLoad && !msg.isRead
        subtitle.fontSize: "medium"
        subtitle.elide: Text.ElideRight
        subtitle.wrapMode: Text.NoWrap

        summary.maximumLineCount: PolicyManager.views.previewLines
        summary.text: canLoad && msg.preview ? msg.preview  : " "
        summary.fontSize: "medium"

        padding.top: units.gu(1)
        padding.bottom: units.gu(1)

        FlipableSelectionAvatar {
            id: inner_name
            msg: normalMessageItemDelegate.msg
            visible: true
            height: units.gu(6)
            width: height
            SlotsLayout.position: SlotsLayout.Leading
            onClicked: {
                if (inner_name.state === "front") {
                    ViewActions.pushToStageArea(ViewKeys.messageListStack, Qt.resolvedUrl("../views/ContactFilterView.qml"),
                                                {
                                                    sender: msg.from,
                                                    title: msg.from.name,
                                                    email: msg.from.address,
                                                    initials: msg.from.initials,
                                                    filterKey: msg.senderMsgKey
                                                }
                                                )
                }
            }
        }

        Item {
            id: inner_infoCol

            property int iconsVerticalSpacing: units.gu(0.5)
            property int iconsHorizontalSpacing: units.gu(1.0)

            height: inner_timeLabel.height + units.gu(1) + star.height
            width: Math.max(inner_timeLabel.width, star.width + attachmentImg.width + units.gu(1))
            SlotsLayout.overrideVerticalPositioning: true

            Label {
                id: inner_timeLabel
                y: main.mainSlot.y + main.title.y
                   + main.title.baselineOffset - baselineOffset
                anchors.right: parent.right
                text: msg ? msg.prettyDate : ""
                fontSize: "small"
            }

            CachedImage {
                id: star
                height: Style.defaultSpacing
                width: height
                anchors {
                    right: parent.right
                    top: inner_timeLabel.bottom
                    topMargin: parent.iconsVerticalSpacing
                }
                name: canLoad && msg.isImportant ? Icons.StarredIcon : Icons.UnStarredIcon
                Suru.highlightType: Suru.WarningHighlight
                color: canLoad && msg.isImportant ? Suru.highlightColor : UbuntuColors.ash
                MouseArea {
                    anchors.fill: parent
                    onClicked: MessageActions.markMessageImportant(msg.messageId, !msg.isImportant)
                }
            }

            CachedImage {
                id: attachmentImg
                height: Style.defaultSpacing
                width: visible ? height : 0
                anchors {
                    right: star.left
                    rightMargin: parent.iconsHorizontalSpacing
                    top: inner_timeLabel.bottom
                    topMargin: parent.iconsVerticalSpacing
                }
                visible: canLoad && msg.hasAttachments
                name: Icons.AttachmentIcon
                color: UbuntuColors.ash
            }
        }
    }
}
