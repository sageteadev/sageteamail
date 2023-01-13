## Client Service Actions {#architecture_client_service_actions}

@startuml{clientserviceactions.svg}

title Client Service Actions

package "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/ClientServiceAction.h src://Dekko/backend/mail/service/ClientServiceAction.h]]" as clientserviceactions {

    class "ClientServiceAction" as clientserviceaction {
    }

    class "DeleteMessagesAction" as deletemessagesaction {
    }

    class "FlagsAction" as flagsaction {
    }

    class "FetchMessagePartAction" as fetchmessagepartaction {
    }

    class "FetchMessagesAction" as fetchmessagesaction {
    }

    class "OutboxAction" as outboxaction {
    }

    class "CreateStandardFoldersAction" as createstandardfoldersaction {
    }

    class "MoveToStandardFolderAction" as movetostandardfolderaction {
    }

    class "MoveToFolderAction" as movetofolderaction {
    }

    class "AccountSyncAction" as accountsyncaction {
    }

    class "EmptyTrashAction" as emptytrashaction {
    }

    class "FolderSyncAction" as foldersyncaction {
    }

    class "SendPendingMessagesAction" as sendpendingmessagesaction {
    }

    class "ExportUpdatesAction" as exportupdatesaction {
    }

    exportupdatesaction --|> clientserviceaction
    sendpendingmessagesaction --|> clientserviceaction
    foldersyncaction --|> clientserviceaction
    emptytrashaction --|> clientserviceaction
    accountsyncaction --|> clientserviceaction
    movetofolderaction --|> clientserviceaction
    movetostandardfolderaction --|> clientserviceaction
    outboxaction --|> clientserviceaction
    fetchmessagesaction --|> clientserviceaction
    fetchmessagepartaction --|> clientserviceaction
    deletemessagesaction --|> clientserviceaction
    flagsaction --|> clientserviceaction
    createstandardfoldersaction --|> clientserviceaction

}

class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/ClientService.h src://Dekko/backend/mail/service/ClientService.h]]" as clientservice {
}

class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/ClientServiceWatcher.h src://Dekko/backend/mail/service/ClientService.h]]" as clientservicewatcher {
}

clientservice --> exportupdatesaction : create\n enqueue
clientservice --> sendpendingmessagesaction : create\n enqueue
clientservice --> foldersyncaction : create\n enqueue
clientservice --> emptytrashaction : create\n enqueue
clientservice --> accountsyncaction : create\n enqueue
clientservice --> movetofolderaction : create\n enqueue
clientservice --> movetostandardfolderaction : create\n enqueue
clientservice --> createstandardfoldersaction : create\n enqueue
clientservice --> outboxaction : create\n enqueue
clientservice --> fetchmessagesaction : create\n enqueue
clientservice --> fetchmessagepartaction : create\n enqueue
clientservice --> deletemessagesaction : create\n enqueue
clientservice --> flagsaction : create\n enqueue

clientserviceaction --> clientservicewatcher : activityChanged

@enduml
