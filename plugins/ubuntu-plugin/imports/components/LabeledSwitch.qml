import QtQuick 2.4
import QtQuick.Controls.Suru 2.2
import Lomiri.Components 1.3


AbstractButton {
    objectName: "labeledSwitch"
    height: units.gu(5)
    width: parent.width
    property alias checked: s.checked
    property alias text: label.text
    property alias label: label
    Label {
        id: label
        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }
        color: Suru.secondaryForegroundColor
    }
    Switch {
        id: s
        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }
    }
    onClicked: s.checked = !s.checked
}
