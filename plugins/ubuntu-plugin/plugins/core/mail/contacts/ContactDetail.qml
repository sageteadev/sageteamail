import QtQuick 2.4
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Ubuntu.Components 1.0

Item {
    property bool editable: false
    property alias title: title.text
    property alias detail: value.text
    property Action detailAction: null
    height: col.height > units.gu(5) ? col.height + units.gu(2) : units.gu(5)
    width: parent.width
    signal valueChanged(string val)

    Column {
        id: col
        spacing: units.gu(1)
        anchors {
            left: parent.left
            leftMargin: units.gu(2)
            top: parent.top
            topMargin: units.gu(2)
            right: actionButton.left
            rightMargin: units.gu(2)

        }

        Label {
            id: title
            visible: text !== ""
            anchors {
                left: parent.left
                right: parent.right
            }
            color: Suru.secondaryForegroundColor
        }
        Label {
            id: value
            visible: !editable && text !== ""
            anchors {
                left: parent.left
                leftMargin: units.gu(1)
                right: parent.right
            }
        }
        TextField {
            id: editField
            visible: editable
            anchors {
                left: parent.left
                right: parent.right
            }
            text: value.text
            onTextChanged: valueChanged(text)
        }
    }
    AbstractButton {
        id: actionButton
        visible: detailAction !== null && !editable
        width: visible ? height : 0
        anchors {
            right: parent.right
            rightMargin: units.gu(2)
            top: parent.top
            topMargin: units.gu(1)
            bottom: parent.bottom
        }
        CachedImage {
            height: units.gu(2.5)
            width: height
            name: Icons.MailUnreadIcon
            anchors.centerIn: parent
        }
        onClicked: detailAction.trigger()
    }
}
