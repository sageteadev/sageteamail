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
import SageteaMail.Ubuntu.Components 1.0
import "./models"

DekkoPage {

    pageHeader.title: qsTr("Licenses")
    extendHeader: !dekko.isSmallFF
    pageHeader.backAction: Action {
        iconName: "back"
        onTriggered: internalStack.pop()
    }

    PageFlickable {
        margins: 0
        spacing: 0
        Repeater {
            model: LicenseModel{}
            delegate: ListItem {

                height: layout.height + divider.height

                ListItemLayout {
                    id: layout
                    title.text: name
                    subtitle.text: license
                }
                onClicked: Qt.openUrlExternally(url)
            }

        }
    }
}

