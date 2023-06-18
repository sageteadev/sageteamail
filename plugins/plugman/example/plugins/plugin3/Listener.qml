import QtQuick 2.4
import Lomiri.Components 1.3
Rectangle {
    width: units.gu(40)
    height: width
    color: LomiriColors.red
    Timer {
        interval: 500
        triggeredOnStart: true
        repeat: true
        onTriggered: console.log("Plugin timer triggered")
        Component.onCompleted: start()
    }
}
