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
import QtQuick.Controls.Suru 2.2
import Lomiri.Components 1.3
import SageteaMail.Components 1.0

AbstractButton {
    id: button

    property alias iconColor: icon.color
    property bool showLabel: true

    readonly property int fullWidth: icon.width + label.width + units.gu(3)
    readonly property int minWidth: icon.width + units.gu(2)

    implicitWidth: width
    implicitHeight: height
    width: showLabel ? fullWidth : minWidth

    LomiriShape {
        anchors {
            topMargin: 0
            bottomMargin: 0
            fill: parent
        }
        aspect: showLabel ? LomiriShape.DropShadow : LomiriShape.Flat
        color: button.pressed ? Suru.neutralColor : Suru.secondaryBackgroundColor
    }

    CachedImage {
        id: icon
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            margins: units.gu(1)
        }
        height: units.gu(2)
        width: height
        name: action.iconName
    }

    Label {
        id: label
        anchors {
            left: icon.right
            leftMargin: units.gu(1)
            verticalCenter: parent.verticalCenter
        }
        text: action.text
        visible: showLabel
    }
}

