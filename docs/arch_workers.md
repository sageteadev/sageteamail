# Workers {#architecture_workers}

@startuml{workers.svg}

title Workers

package "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers src://Dekko/workers]]" as workers {
    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/Workers.qml Workers.qml]]" as workerregistry {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/AccountsWorker.qml AccountsWorker.qml]]" as accountsworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/ComposerWorker.qml ComposerWorker.qml]]" as composerworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/MailboxWorker.qml MailboxWorker.qml]]" as mailboxworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/MailWorker.qml MailWorker.qml]]" as mailworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/SettingsWorker.qml SettingsWorker.qml]]" as settingsworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/ErrorsWorker.qml ErrorsWorker.qml]]" as errorsworker {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/UriWorker.qml UriWorker.qml]]" as uriworker {
    }

    class "ViewWorker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/app/Dekko.qml Dekko.qml]]" as viewworker {
    }

    accountsworker *-- workerregistry
    mailboxworker *-- workerregistry
    settingsworker *-- workerregistry
    errorsworker *-- workerregistry
    uriworker *-- workerregistry
}

package "[[architecture_stores.html stores]]" as stores {

}


() "Actions\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/api/public src://Dekko/api/public]]" as actions

actions --> workers : are processed
workers --> stores : dispatches events

@enduml

