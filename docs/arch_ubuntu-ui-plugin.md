# Ubuntu Touch UI {#architecture_ubuntu_ui}

@startuml{ubuntu-ui.svg}

title Ubuntu Touch UI

package "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin src://plugins/ubuntu-plugin]]" as utui {
    component "[[architecture_composer.html Composer]]\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer src://plugins/ubuntu-plugin/plugins/core/mail/composer]]" as composer

    component "Accounts\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/accounts src://plugins/ubuntu-plugin/plugins/core/mail/accounts]]" as accounts

    component "Messageview\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/messageview src://plugins/ubuntu-plugin/plugins/core/mail/messageview]]" as messageview

    component "Settings\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/settings src://plugins/ubuntu-plugin/plugins/core/mail/settings]]" as settings

    component "SetupWizard\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/setupwizard src://plugins/ubuntu-plugin/plugins/core/mail/setupwizard]]" as setupwizard

    component "MailStage\n[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/MailStage.qml src://plugins/ubuntu-plugin/plugins/core/mail/MailStage.qml]]" as mailstage
}

package "[[architecture_stores.html stores]]" as stores {
}
stores --> utui : delivers state

() "Actions\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/api/public src://Dekko/api/public]]" as actions
utui --> actions : generates

() "Content-Hub" as contenthub

utui --> contenthub : shares data
contenthub --> utui : provides data

:User: --> utui : touches, puts in
utui --> :User: : views state

@enduml

