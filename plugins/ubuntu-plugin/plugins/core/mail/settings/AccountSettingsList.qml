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
import SageteaMail.Mail.Stores.Settings 1.0
import PlugMan 1.0
import SageteaMail.Ubuntu.Components 1.0

DekkoPage {
    pageHeader.title: qsTr("Account Settings: %1").arg(SettingsStore.selectedAccount.name)
    pageHeader.backAction: Action {
        iconName: "back"
        onTriggered: {
            SettingsActions.saveCurrentGroup()
            if (SettingsStore.settingsChanged) {
                SettingsActions.saveSelectedAccount()
                ViewActions.reloadAccountBasedModels()
                SettingsStore.settingsChanged = false
            }
            ViewActions.popStageArea(ViewKeys.settingsStack2)
            ViewActions.popStageArea(ViewKeys.settingsStack1)
        }
    }

    pageHeader.primaryAction: Action {
        iconName: "delete"
        onTriggered: {
            AccountActions.deleteAccount(SettingsStore.selectedAccountId, SettingsStore.selectedAccount.name, true)
            ViewActions.popStageArea(ViewKeys.settingsStack2)
            ViewActions.popStageArea(ViewKeys.settingsStack1)
        }

    }

    ActionRegistry {
        id: registry
        location: "SageteaMail::Settings::AccountAction"
        defaultActions: [
            Action {
                text: qsTr("Details")
                onTriggered: SettingsActions.openSettingsGroup(Qt.resolvedUrl("./DetailsGroup.qml"))
            },
            Action {
                text: qsTr("Incoming Server")
                onTriggered: SettingsActions.openSettingsGroup(Qt.resolvedUrl("./IncomingServerGroup.qml"))
            },
            Action {
                text: qsTr("Outgoing Server")
                onTriggered: SettingsActions.openSettingsGroup(Qt.resolvedUrl("./OutgoingServerGroup.qml"))
            },
            Action {
                text: qsTr("Copies and Folders")
                onTriggered: SettingsActions.openSettingsGroup(Qt.resolvedUrl("./CopyFoldersGroup.qml"))
            }
        ]
    }

    PageFlickable {
        margins: 0
        spacing: 0

        Repeater {
            model: registry.actions
            delegate: ListItem {
                id: d
                action: modelData
                height: l1.height + divider.height
                ListItemLayout {
                    id: l1
                    title.text: d.action.text
                    ProgressionSlot{}
                }
            }
        }
    }
}
