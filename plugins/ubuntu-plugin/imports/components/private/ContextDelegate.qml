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
import QtQuick.Controls.Suru 2.2

import Ubuntu.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Ubuntu.Components 1.0

ListItem {
    id: root
    height: ctxtLayout.height
    divider.visible: false
    color: Suru.secondaryBackgroundColor
    property string description
    property alias actionIcon: leadingImage.name
    property alias actionIconColor: leadingImage.color
    property alias selectedIcon: trailingImage.name
    property alias selectedIconColor: trailingImage.color
    property alias selected: trailingImage.visible

    // So aparently trying to show a highlight on mouse over is
    // a partcularly annoying thing! Just binding to MouseArea::containsMouse
    // results in all delegates being highlighted on the context menu's completion
    // So instead we have to late bind containsMouse using a short timer binding
    // containsMouse to the highlight rectangle.
    Rectangle {
        id: h
        property bool canShow: false
        anchors.fill: parent
        visible: false
        color: Suru.neutralColor
    }
    MouseArea {
        id: ma
        anchors.fill: h
        onClicked: root.clicked()
    }
    Binding {
        target: h
        property: "visible"
        value: (ma.containsMouse || ma.pressed)
        when: h.canShow
    }
    Timer {
        id: bindTimer
        interval: 250
        repeat: false
        onTriggered: {
            h.canShow = true
            ma.hoverEnabled = true
        }
    }

    ListItemLayout {
        id: ctxtLayout
        height: units.gu(5)
        title.text: description
        CachedImage {
            id: leadingImage
            height: units.gu(3); width: height
            SlotsLayout.position: SlotsLayout.Leading
        }
        CachedImage {
            id: trailingImage
            height: units.gu(3); width: height
        }
    }



    Component.onCompleted: bindTimer.start()
}
