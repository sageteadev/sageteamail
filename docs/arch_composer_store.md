## Composer Store {#architecture_composer_store}

@startuml{composer_store.svg}

title Composer Store

package "[[architecture_stores.html stores]]" {

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Composer/ComposerStore.qml Composer/ComposerStore.qml]]" as composerstore {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Accounts/AccountStore.qml Accounts/AccountStore.qml]]" as accountstore {
    }
}


package "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend src://Dekko/backend]]" {
    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/SubmissionManager.h mail/SubmissionManager.h]]" as submissionmanager {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/MessageBuilder.h mail/MessageBuilder.h]]" as messagebuilder {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/SenderIdentities.h mail/SenderIdentities.h]]" as senderidentities {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceClient.h mail/service/MailServiceClient.h]]" as mailserviceclient {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceInterface.h mail/service/MailServiceInterface.h]]" as mailserviceinterface {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/accounts/Account.h accounts/Account.h]]" as account {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/accounts/AccountConfiguration.h accounts/AccountConfiguration.h]]" as accountconfiguration {
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/accounts/Identities.h accounts/Identities.h]]" as identities {
    }

    account "*" *-- senderidentities
    identities *-- senderidentities
    mailserviceinterface *-- mailserviceclient

    submissionmanager --> mailserviceclient : markMessageForwarded\n markMessageReplied\n sendMessage\n addMessage\n removeMessage\n moveToStandardFolder

    mailserviceclient --> submissionmanager : messageSent

    mailserviceinterface --> mailserviceclient : messagesSent\n messageSendingFailed

    mailserviceclient --> mailserviceinterface : removeMessage\n sendMessage\n markMessageForwarded\n markMessageReplied\n moveToStandardFolder 
}


cloud "DBus" as dbus {

}

frame "[[architecture_dekko_worker.html dekko-worker]]" as dekkoworker {
    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/backend/mail/service/MailServiceAdaptor.h mail/service/MailServiceAdaptor.h]]" as mailserviceadaptor {
    }
}

mailserviceinterface --|> QDBusAbstractInterface
mailserviceinterface <--> dbus
dbus <--> mailserviceadaptor

composerstore --* senderidentities
composerstore --* messagebuilder
composerstore --* submissionmanager

messagebuilder --> composerstore : attachments\n subject\n body

senderidentities --> composerstore : selectedAccount.identity\n model\n selectedIndex

accountstore --> senderidentities : sendAccountModel\n identities

senderidentities o-- messagebuilder

messagebuilder o-- submissionmanager
messagebuilder --> submissionmanager : message
messagebuilder <--> submissionmanager : misc

package "[[architecture_workers.html workers]]" {
    class "ComposerWorker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/ComposerWorker.qml src://Dekko/workers/ComposerWorker.qml]]" as composerworker {
    }

    class "ViewWorker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/app/SageteaMail.qml src://Dekko/app/SageteaMail.qml]]" as viewworker {
    }
}

submissionmanager --> composerworker : resetComposer
submissionmanager --> viewworker : closeComposer\n orderSimpleToast

composerstore --> composerworker : discardMessage
composerstore --> submissionmanager : send

package "[[https://gitlab.com/dekkan/messagingframework/-/tree/master/ src://messagingframework/]]" {

    class "QMailStore\n[[https://gitlab.com/dekkan/messagingframework/-/tree/master/libraries/qmfclient/qmailstore.h src://libraries/qmfclient/qmailstore.h]]" as qmailstore {
    }

    class "QMailAccount\n[[https://gitlab.com/dekkan/messagingframework/-/tree/master/libraries/qmfclient/qmailaccount.h src://libraries/qmfclient/qmailaccount.h]]" as qmailaccount {
    }

    class "QMailAccountConfiguration\n[[https://gitlab.com/dekkan/messagingframework/-/tree/master/libraries/qmfclient/qmailaccountconfiguration.h src://libraries/qmfclient/qmailaccountconfiguration.h]]" as qmailaccountconfiguration {
    }

    class "QMailServiceConfiguration\n[[https://gitlab.com/dekkan/messagingframework/-/tree/master/libraries/qmfmessageserver/qmailserviceconfiguration.h src://libraries/qmfmessageserver/qmailserviceconfiguration.h]]" as qmailserviceconfiguration {
    }
}

qmailaccount *-- account
qmailaccountconfiguration *-- account
accountconfiguration "2" *-- account
qmailaccountconfiguration *-- accountconfiguration
qmailserviceconfiguration *-- accountconfiguration

mailserviceclient --> qmailstore : addMessage

@enduml

