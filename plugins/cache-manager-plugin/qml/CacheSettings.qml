import QtQuick 2.4
import Ubuntu.Components 1.3
import SageteaMail.Mail.API 1.0

Action {
    iconName: "preferences-desktop-notifications-symbolic"
    text: qsTr("Manage cache")
    onTriggered: ViewActions.pushToStageArea(
                    ViewKeys.settingsStack1,
                    Qt.resolvedUrl("./CacheSettingsPage.qml"),
                    {}
                 )
}
