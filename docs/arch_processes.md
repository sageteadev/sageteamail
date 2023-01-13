%Dekkos Processes     {#architecture_processes}
=================

@startuml{processes.svg}

title Dekkos Processes

node "Dekkos processes" {

    frame "[[architecture_dekko.html dekko]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/app src://Dekko/app]]" as dekko {
    }

    frame "[[architecture_dekko_worker.html dekko-worker]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/server/serviceworker src://Dekko/server/serviceworker]]" as dekkoworker {
    }
 
    frame "dekkod\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/server src://Dekko/server]]" as dekkod {
    } 

    frame "dekkod-notify\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-notification-plugin src://plugins/ubuntu-notification-plugin]]" as dekkodnotify {
    }
}

frame "Upstart User Session" as upstart {
}

frame "System Notification Center" as sysnotifcenter {
}

cloud "Mail server" as mailserver{
    [IMAP]
    [SMTP]
}


:User: --> dekko : starts, uses UI, suspends, closes
dekko --> dekkoworker : starts
dekko --> dekkod : restarts
dekko --> dekkodnotify : restarts
upstart --> dekkod : starts, stops
upstart --> dekkodnotify : starts, stops
dekkod --> mailserver : sends, fetches and syncs mail
dekkodnotify --> sysnotifcenter : displays notifications

@enduml

