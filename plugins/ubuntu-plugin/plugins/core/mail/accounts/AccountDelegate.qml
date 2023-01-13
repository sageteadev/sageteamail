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
import SageteaMail.Mail.API 1.0
import SageteaMail.Ubuntu.Components 1.0

ListItem {

    property var account
    property alias showProgression: ps.visible
    property bool canDelete: false
    property bool showContextActions: false
    divider.visible: false
    height: layout.height + line.height

    ListItemLayout {
        id: layout
        title.text: account ? account.name : ""
        Icon {
            height: units.gu(2.5)
            width: height
            name: "contact"
            SlotsLayout.position: SlotsLayout.Leading
        }
        ProgressionSlot {
            id: ps
        }
    }
    leadingActions: canDelete ? leading : null

    ListItemActions {
        id: leading
        actions: [
            Action {
                iconName: "delete"
                onTriggered: AccountActions.deleteAccount(account.id, account.name, true)
            }
        ]
    }

    Line {
        id: line
        anchors {
            left: parent.left
            bottom: parent.bottom
            right: parent.right
        }
    }
}
