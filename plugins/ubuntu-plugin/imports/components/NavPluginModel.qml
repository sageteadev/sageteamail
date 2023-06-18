import QtQuick 2.4
import PlugMan 1.0
import Lomiri.Components 1.3
import SageteaMail.Mail.API 1.0

Item {

    property alias plugins: navRegistry.actions


    ItemRegistry {
        id: registry
    }

    // Mail stage is special here as it's the only one kept on the stack
    // all other stage plugins are destroyed on stage push's
    // If you want your stage to maintain state then you should make
    // use of the store/worker pattern and use MazDB or MazDBSettings for data persistence
    ActionRegistry {
        id: navRegistry
        location: "SageteaMail::Stage::Action"
        defaultActions: [
            Action {
                iconName: "mail-unread"
                onTriggered: ViewActions.rewindStageStack()
            }
        ]
    }

}
