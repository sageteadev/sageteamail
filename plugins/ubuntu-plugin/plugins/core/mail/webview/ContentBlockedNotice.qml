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
import SageteaMail.Ubuntu.Constants 1.0

Item {
    id: notice

    property bool contentBlocked: false
    readonly property bool isExpanded: state === "expanded"

    signal allowClicked()

    ListItem {
        anchors.fill: parent
        divider.visible: notice.contentBlocked
        color: Suru.secondaryBackgroundColor
        Label {
            anchors {
                left: parent.left
                leftMargin: Style.defaultSpacing
                verticalCenter: parent.verticalCenter
                right: allowButton.left
                rightMargin: units.gu(1.5)
            }
            elide: Text.ElideRight
            clip: true
            visible: notice.contentBlocked
            text: qsTr("Remote content blocked")
        }

        Button {
            id: allowButton
            height: units.gu(4)
            width: allowAction.text.width > units.gu(15) ? units.gu(15) : allowAction.text.width
            anchors {
                right: parent.right
                rightMargin: Style.defaultSpacing
                verticalCenter: parent.verticalCenter
            }
            Suru.highlightType: Suru.NegativeHighlight
            color: Suru.highlightColor
            action: Action {
                id: allowAction
                text: qsTr("Allow")
                onTriggered: notice.allowClicked()
            }
        }
    }

    state: contentBlocked ? "expanded" : "collapsed"
    states: [
        State {
            name: "collapsed"
            PropertyChanges {
                target: notice
                height: 0
            }
        },
        State {
            name: "expanded"
            PropertyChanges {
                target: notice
                height: units.gu(6)
            }
        }
    ]
    // We only want to animate on closing. Looks a bit weird
    // on expanding while the view is still being constructed.
    transitions: [
        Transition {
            from: "expanded"
            to: "collapsed"
            animations: [
                NumberAnimation {
                    target: notice
                    property: "height"
                    duration: UbuntuAnimation.FastDuration
                }
            ]
        }
    ]
}
