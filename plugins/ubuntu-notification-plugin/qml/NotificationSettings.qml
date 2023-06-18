import QtQuick 2.4
import Lomiri.Components 1.3
import SageteaMail.Mail.API 1.0

Action {
    iconName: "preferences-desktop-notifications-symbolic"
    text: qsTr("Notifications")
    onTriggered: ViewActions.pushToStageArea(
                    ViewKeys.settingsStack1,
                    Qt.resolvedUrl("./NotificationSettingsPage.qml"),
                    {}
                 )
}
