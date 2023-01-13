# Dekko main process {#architecture_dekko}

@startuml{dekko.svg}

title Main process (dekko)

frame "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/ src://Dekko/app]]" as dekko {

    package "plugman" {
    }

    package "[[architecture_ubuntu_ui.html ubuntu-ui-plugin]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin src://plugins/ubuntu-plugin]]" as utui {
    }

    package "[[architecture_stores.html stores]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores src://Dekko/stores]]" as stores {
    }

    package "[[architecture_workers.html workers]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers src://Dekko/workers]]" as workers {
    }

    package "backend\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend src://Dekko/backend]]" as backend {
    }

    () "actions\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/api/public src://Dekko/actions]]" as actions

    utui --> actions : generates
    actions --> workers : are processed
    workers --> stores : dispatches events
    stores --> utui : delivers state
    stores --> backend
    backend --> stores
}

() "Content-Hub" as contenthub

utui --> contenthub : shares data
contenthub --> utui : provides data

:User: --> utui : touches, puts in
utui --> :User: : views state

frame "[[architecture_dekko_worker.html dekko-worker]]" as dekkoworker {
}
backend <--> dekkoworker : sends, fetches and syncs mail

@enduml

