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
import SageteaMail.Components 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Ubuntu.Constants 1.0
import SageteaMail.Ubuntu.Components 1.0

ListItemWithActions {
    id: listItem
    property Component openItem: Item{}
    property var attachment: model.qtObject
    width: parent.width
    showDivider: true
    triggerIndex: model.index

    Item {
        Connections {
            target: attachment
            onReadyToOpen: {
                if (isRunningOnMir) {
                    ContentActions.exportFile(dekko, url)
                } else {
                    Qt.openUrlExternally(url)
                }
            }
        }
    }

    onItemClicked: {
        Log.logInfo("AttachmentPanel::openAttachment", "Attachment octet size is: %1".arg(attachment.sizeInBytes))
        // This really is a crude hack
        // the attachments object can't directly
        // access the custom qnam factory. So we pass
        // it a QObject from the qml context which
        // the attachment object can access the QQmlEngine from
        // FIXME: Refactor Client* classes into seperate lib
        attachment.open(openItem.createObject())
    }

    ListItemLayout {
        id: aLayout
        height: listItem.height
        title.text: attachment ? attachment.displayName : ""
        subtitle.text: attachment ? attachment.mimeType + ", " + attachment.size : ""

        Icon {
            source: Paths.mimeIconForMimeType(attachment.mimeType)
            color: "#888888"
            height: Style.largeSpacing; width: height
            SlotsLayout.position: SlotsLayout.Leading
        }

        ActivityIndicator {
            visible: running
            running: attachment.fetchInProgress
        }
    }
}
