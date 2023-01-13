## Dekko Worker {#architecture_dekko_worker}

@startuml{dekkoworker.svg}

title dekko-worker

frame "[[https://gitlab.com/dekkan/messagingframework/-/tree/master/libraries/qmfclient qtmessageframeworkclient]]" as qmf {
    class "QMailStore" as qmailstore {
    }

    class "QMailDisconnected" as qmaildisconnected {
    }

    class "QMailMessageKey" as qmailmessagekey {
    }

    class "QMailServiceAction" as qmailserviceaction {
    }
}

note top of qmf : Not all dependencies to qmf are depicted

frame "dekko-worker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/server/serviceworker src://Dekko/server/serviceworker]]" {
    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceAdaptor.h mail/service/MailServiceAdaptor.h]]" as mailserviceadaptor {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceWorker.h mail/service/MailServiceWorker.h]]" as mailserviceworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/ClientService.h mail/service/ClientService.h]]" as clientservice {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/ClientService.h ClientServiceWatcher]]" as clientservicewatcher {
    }

    mailserviceworker *-- mailserviceadaptor : parent of
    clientservice *-- mailserviceworker
    clientservicewatcher *-- clientservice

    package "[[architecture_client_service_actions.html Client Service Actions]]" as clientserviceactions {
    }

    clientserviceactions --> clientservicewatcher : activityChanged
    clientservicewatcher --> clientservice : messagePartFetched\n messageFetched\n accountSynced\n standardFoldersCreated\n checkSendMailQueue\n messageSent\n messagePartFetchFailed\n messageFetchFailed\n syncAccountFailed\n messageSendingFailed\n processNext\n actionFailed
    clientservice --> clientserviceactions : create\n enqueue\n process\n isRunning

    clientservice --> mailserviceworker : undoCountChanged\n updatesRolledBack\n messagePartFetched\n messagePartFetchFailed\n messageFetched\n messageFetchFailed\n messageSent\n messageSendingFailed\n accountSynced\n syncAccountFailed\n actionFailed\n standardFoldersCreated

    mailserviceworker --> clientservice : deleteMessages\n restoreMessages\n markMessagesImportant\n markMessagesRead\n markMessagesTodo\n markMessagesDone\n markMessagesReplied\n markMessageForwarded\n syncFolders\n createStandardFolders\n moveToFolder\n moveToStandardFolder\n markFolderRead\n downloadMessagePart\n downloadMessages\n sendMessage\n sendAnyQueuedMail\n synchronizeAccount\n undoActions\n emtpyTrash\n removeMessage

    mailserviceworker --> qmailstore : countMessages\n queryMessages\n queryFolders\n removeMessages
}

clientservice --> qmaildisconnected : restoreToPreviousFolder\n flagMessages\n updatesOutstanding\n rollBackUpdates
clientservice --> qmailstore : updateMessage\n removeMessage\n queryMessages\n messagesMetaData\n queryAccounts
clientservice --> qmailmessagekey : status\n parentFolderId
qmailserviceaction --> clientserviceactions : activityChanged
qmailserviceaction <|.. clientserviceactions : uses
clientserviceactions --> qmaildisconnected : moveToStadardFolders\n flagMessages\n moveToFolder\n moveToStandardFolder

frame "[[architecture_dekko.html dekko]]" {
    package "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend src://Dekko/backend]]" {
        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceInterface.h mail/service/MailServiceInterface.h]]" as mailserviceinterface {
        }
    }
}

cloud "DBus" as dbus {

}

mailserviceinterface --|> QDBusAbstractInterface
mailserviceinterface <--> dbus : data

dbus --> mailserviceadaptor : createStandardFolders\n deleteMessages\n downloadMessagePart\n downloadMessages\n emptyTrash\n markFolderRead\n markMessageForwarded\n markMessagesDone\n markMessagesImportant\n markMessagesRead\n markMessagesReplied\n markMessagesTodo\n moveToFolder\n moveToStandardFolder\n pruneCache\n queryFolders\n queryMessages\n removeMessage\n restoreMessage\n sendQueuedMail\n sendMessage\n sendPendingMessages\n syncFolders\n synchronizeAccount\n totalCount\n undoActions

mailserviceadaptor --> dbus : accountSynced\n actionFailed\n messageFetchFailed\n messagePartFetchFailed\n messagePartNowAvailable\n messageNowAvailable\n messageRestored\n messageSendingFailed\n messagesSent\n queueChanged\n standardFoldersCreated\n syncAccountFailed\n undoCountChanged\n updatesRolledBack

mailserviceadaptor --> mailserviceworker : createStandardFolders\n deleteMessages\n downloadMessagePart\n downloadMessages\n emptyTrash\n markFolderRead\n markMessageForwarded\n markMessagesDone\n markMessagesImportant\n markMessagesRead\n markMessagesReplied\n markMessagesTodo\n moveToFolder\n moveToStandardFolder\n pruneCache\n queryFolders\n queryMessages\n removeMessage\n restoreMessage\n sendQueuedMail\n sendMessage\n sendPendingMessages\n syncFolders\n synchronizeAccount\n totalCount\n undoActions\n hasUndoableActions\n undoDescription

mailserviceworker --> mailserviceadaptor : ? ToDo ?

@enduml

