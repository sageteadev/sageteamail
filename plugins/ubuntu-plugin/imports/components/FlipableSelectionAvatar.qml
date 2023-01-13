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
import QtFeedback 5.0
import Ubuntu.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Mail 1.0
import "./private/UiUtils.js" as UiUtils
import SageteaMail.Ubuntu.Constants 1.0

Flipable {
    id: flipable
    property var msg

    signal clicked()

    front: PixelPerfectItem {
        id: inner_avatar
        anchors.fill: parent
        Component {
            id: component_inner_av
            UbuntuShape {
                id: avatarCircle
                anchors.fill: parent
                anchors.centerIn: parent
                aspect: UbuntuShape.Flat
                backgroundColor: msg ? UiUtils.getIconColor(msg.from.name) : "transparent"
                opacity: ma.containsMouse ? 0.8 : 1.0

                Label {
                    id: initialsLabel
                    color: Suru.backgroundColor
                    text: msg ? msg.from.initials : ""
                    anchors.centerIn: parent
                    fontSize: "large"
                }
                Icon {
                    id: contactIcon
                    name: "contact"
                    anchors.centerIn: parent
                    height: parent.height - Style.defaultSpacing
                    width: height
                    color: Suru.backgroundColor
                    visible: !initialsLabel.text
                }
            }
        }
        Loader {
            id: loader_inner_av
            anchors.fill: parent
            active: !inner_name.visible
            sourceComponent: component_inner_av
        }
        UbuntuShapeGravatar {
            id: inner_name
            emailAddress: msg ? msg.from.address : ""
            clip: true
            anchors.fill: parent
            visible: status === Image.Ready
            opacity: ma.containsMouse ? 0.8 : 1.0
        }

        MouseArea {
            id: ma
            hoverEnabled: true
            anchors.fill: parent
            visible: flipable.state === "front"
            onClicked: flipable.clicked()
            // For some weird reason we can't just bind
            // avatarCircle to containsMouse. As during
            // pushing and popping another page to the stack
            // can cause multiple avatars with the search overaly visible.
            // It's mighty weird!!!
            // So instead we just manually show/hide when mouse enters/exits
            // Not ideal but it enforces the overlay is only visible when we want
            onEntered: {
                if (containsMouse) {
                    avatarSearchOverlay.visible = true
                }
            }
            onExited: {
                if (!containsMouse) {
                    avatarSearchOverlay.visible = false
                }
            }
        }

        UbuntuShape {
            id: avatarSearchOverlay
            anchors.fill: parent
            anchors.centerIn: parent
            aspect: UbuntuShape.Flat
            backgroundColor: Suru.secondaryForegroundColor
            opacity: 0.8
            visible: false
            CachedImage {
                width: parent.width - Style.defaultSpacing
                height: width
                anchors.centerIn: parent
                name: Icons.SearchIcon
                color: Suru.backgroundColor
            }
        }
    }

    back: PixelPerfectItem {
        anchors.fill: parent
        ListItemWithActionsCheckBox {
            anchors.centerIn: parent
            checked: msg && msg.checked
        }
        
        MouseArea {
            id: mA
            hoverEnabled: true
            anchors.fill: parent
            visible: flipable.state === "back"
            onClicked: {
                clickEffect.start()
                if (msg.checked) {
                    MessageActions.setMessageCheck(model.index, Qt.Unchecked)
                } else {
                    MessageActions.setMessageCheck(model.index, Qt.Checked)
                }
            }
        }
    }

    transform: Rotation {
        id: rotation
        origin.x: flipable.width/2
        origin.y: flipable.height/2
        axis.x: 0; axis.y: 1; axis.z: 0
    }

    state: MailStore.isInSelectionMode ? "back" : "front"

    states: [
        State {
            name: "front"
            PropertyChanges { target: rotation; angle: 0 }

        },
        State {
            name: "back"
            PropertyChanges { target: rotation; angle: 180 }
        }
    ]

    transitions: Transition {
        SequentialAnimation {
            PauseAnimation {
                duration: {
                    var result;
                    if (model.index > listView.selectionIndex) {
                        result = model.index - listView.selectionIndex
                    } else if ( model.index < listView.selectionIndex) {
                        result = listView.selectionIndex - model.index
                    }
                    if (result > 0 && result <= 20) {
                        return result * 100
                    } else {
                        return 0
                    }
                }
            }
            NumberAnimation { target: rotation; property: "angle"; duration: UbuntuAnimation.FastDuration }
        }
    }
    
    HapticsEffect {
        id: clickEffect
        attackIntensity: 0.0
        attackTime: 50
        intensity: 1.0
        duration: 10
        fadeTime: 50
        fadeIntensity: 0.0
    }
}
