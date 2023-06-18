import QtQuick 2.4
import QtQuick.Controls.Suru 2.2
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3

PopupBase {
    id: base

    property alias text: label.text

    Rectangle {
        anchors.fill: parent
        color: Suru.foregroundColor
        opacity: 0.9
    }

    Item {
        anchors.centerIn: parent
        width: parent.width
        height: label.height + spinner.height + units.gu(3)
        Label {
            id: label
            color: Suru.backgroundColor
            anchors.horizontalCenter: parent.horizontalCenter
        }
        ActivityIndicator {
            id: spinner
            running: true
            anchors {
                top: label.bottom
                topMargin: units.gu(3)
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
