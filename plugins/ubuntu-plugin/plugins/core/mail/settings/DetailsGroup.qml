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
import QuickFlux 1.0
import Lomiri.Components 1.3
import SageteaMail.AutoConfig 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Settings 1.0
import SageteaMail.Lomiri.Components 1.0

SettingsGroupPage {
    pageHeader.title: qsTr("Details: %1").arg(SettingsStore.selectedAccount.name)

    AppListener {
        Filter {
            type: SettingsKeys.saveCurrentGroup
            onDispatched: {
                Log.logInfo("DetailsGroup::saveCurrentGroup", "Saving current group")
                if (account.name === accountName.text) {
                    Log.logInfo("DetailsGroup::saveCurrentGroup", "No settings changed")
                    return
                }
                account.name = accountName.text
                SettingsStore.settingsChanged = true
                Log.logInfo("DetailsGroup::saveCurrentGroup", "Current group saved")
            }
        }
    }

    PageFlickable {
        TitledTextField {
            id: accountName
            title: qsTr("Account name")
            text: account.name
        }
    }
}
