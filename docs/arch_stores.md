# Stores {#architecture_stores}

@startuml{stores.svg}

title Stores

package "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores src://Dekko/stores]]" as stores {

    class "AccountStore\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Accounts/AccountStore.qml AccountStore.qml]]" as accountstore 

    class "[[architecture_composer_store.html ComposerStore]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Composer/ComposerStore.qml ComposerStore.qml]]" as composerstore 

    class "MailboxStore\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Mail/MailboxStore.qml MailboxStore.qml]]" as mailboxstore 

    class "MailStore\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Mail/MailStore.qml MailStore.qml]]" as mailstore 

    class "SettingsStore\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Settings/SettingsStore.qml SettingsStore.qml]]" as settingsstore 

    class "ViewsStore\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Views/ViewStore.qml ViewStore.qml]]" as viewstore 

}

package "[[architecture_workers.html workers]]" as workers {
    class dummy
}

package "[[architecture_ubuntu_ui.html ubuntu-ui-plugin]]" as utui {
    class dummy
}

workers --> stores : dispatches events
stores --> utui : delivers state

@enduml

