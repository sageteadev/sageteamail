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
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Settings 1.0
import PlugMan 1.0
import SageteaMail.Lomiri.Components 1.0

DekkoPage {
    pageHeader.title: qsTr("Manage cache: %1").arg(SettingsStore.selectedAccount.name)
    pageHeader.backAction: Action {
        iconName: "back"
        onTriggered: {
            ViewActions.popStageArea(ViewKeys.settingsStack1)
        }
    }

    readonly property var periods: [
        qsTr("One Week"),
        qsTr("Fortnight"),
        qsTr("One Month"),
        qsTr("Three Months"),
        qsTr("Six Months")
    ]

    PageFlickable {
        margins: 0
        ListItemLayout {
            title.text: qsTr("Clear messages from cache")
            summary.text: qsTr("This will clear messages older than the given period")
            summary.color: theme.palette.normal.negative
            summary.wrapMode: Text.WordWrap
        }

        OptionSelector {
            id: selector
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: units.gu(1)
                rightMargin: units.gu(1)
            }
            model: periods
            delegate: OptionSelectorDelegate {
                text: modelData
            }
        }

        Item {
            width: parent.width
            height: units.gu(5)
            Button {
                anchors {
                    right: parent.right
                    rightMargin: units.gu(1)
                    verticalCenter: parent.verticalCenter
                }
                color: theme.palette.normal.negative
                text: qsTr("Clear now")
                onClicked: service.sendMessage("cache-manager", "prune:%1".arg(SettingsStore.selectedAccountId), selector.selectedIndex);
            }
        }
    }
}
