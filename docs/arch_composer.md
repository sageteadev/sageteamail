## Composer {#architecture_composer}

@startuml{composer.svg}

title Composer

package "[[architecture_ubuntu_ui.html ubuntu-ui-plugin]]" {

    package "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/Composer.qml src://plugins/ubuntu-plugin/plugins/core/mail/composer]]" as ccomposer {

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/Composer.qml Composer.qml]]" as composer {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/Composer.qml ComposeSidePanel.qml]]" as composesidepanel {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/AttachmentList.qml AttachmentList.qml]]" as attachmentlist {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/MessageComposer.qml MessageComposer.qml]]" as messagecomposer {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/RecipientField.qml RecipientField.qml]]" as recipientfield {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/SubjectField.qml SubjectField.qml]]" as subjectfield {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/SenderIdentityField.qml SenderIdentityField.qml]]" as senderidentityfield {
        }

        class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/plugins/core/mail/composer/ComposeWindow.qml ComposeWindow.qml]]" as composewindow {
        }

        composer o-- composewindow : loaded from ItemRegistry
        composesidepanel *-- composer
        messagecomposer *-- composer
        senderidentityfield *-- messagecomposer
        recipientfield "3" *-- messagecomposer
        subjectfield *-- messagecomposer
        attachmentlist *-- composesidepanel
    }

    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/plugins/ubuntu-plugin/qml/workers/ContentWorker.qml ContentWorker.qml]]" as contentworker {
    }

    composer --> contentworker : pickFile
}

package "[[architecture_stores.html stores]]" {
    class "[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/stores/Composer/ComposerStore.qml src://Dekko/stores/Composer/ComposerStore.qml]]" as composerstore
}

composerstore --> composer : hasValidIdentity
composerstore --> messagecomposer : bodyDocument\n currentSignature
composerstore --> composesidepanel : sidePanelOpen (attachments)
composerstore --> composewindow : builder.hasDraft
composerstore --> attachmentlist : attachments
composerstore --> subjectfield : subjectDocument
composerstore --> senderidentityfield : identitiesModel\n identityIndex\n identity
composerstore --> recipientfield : showCC\n showBCC\n recipients


package "[[architecture_workers.html workers]]" {
    class "ComposerWorker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/workers/ComposerWorker.qml src://Dekko/workers/ComposerWorker.qml]]" as composerworker {
    }

    class "ViewWorker\n[[https://gitlab.com/dekkan/dekko/-/tree/master/Dekko/app/Dekko.qml src://Dekko/app/Dekko.qml]]" as viewworker {
    }
}

composer --> composerworker : discardMessageAction\n saveDraftAction\n sendAction
composewindow --> composerworker : discardMessage, resetComposer
composewindow --> viewworker : closeComposer
attachmentlist --> composerworker : removeAttachment
senderidentityfield --> composerworker : setIdentity
recipientfield --> composerworker : validateAddress\n addRecipientFromAddress
composerworker --> recipientfield : validAddress\n invalidAddress

() "Content-Hub" as contenthub

contenthub --> contentworker : importRequest, shareRequest
contentworker --> contenthub : pick content

contentworker --> composerworker : addFileAttachement\n appendTextToSubject\n appendTextToBody

contentworker --> viewworker : openComposer

@enduml

