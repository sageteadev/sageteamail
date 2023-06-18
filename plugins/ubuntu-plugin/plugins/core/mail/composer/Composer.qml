import QtQuick 2.4
import QtQuick.Controls.Suru 2.2
import Lomiri.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Composer 1.0
import QuickFlux 1.0
import PlugMan 1.0
import SageteaMail.Lomiri.Components 1.0
import SageteaMail.Lomiri.Constants 1.0
import SageteaMail.Lomiri.Helpers 1.0
import "../components"

ViewState {
    id: composer
    anchors.fill: parent

    StretchColumn {
        anchors.fill: parent

        Rectangle {
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
            }
            height: units.gu(6)
            implicitHeight: height
            color: Suru.secondaryBackgroundColor
            StretchRow {
                id: row
                anchors.fill: parent
                anchors.leftMargin: units.gu(1)
                anchors.rightMargin: units.gu(1)
                spacing: units.gu(1)
                readonly property bool textVisible: composer.width > units.gu(60)
                TitledHeaderButton {
                    height: units.gu(4)
                    anchors.verticalCenter: parent.verticalCenter
                    action: ComposerStore.actions.discardMessageAction
                    showLabel: row.textVisible
                }
                TitledHeaderButton {
                    height: units.gu(4)
                    anchors.verticalCenter: parent.verticalCenter
                    visible: ComposerStore.hasValidIdentity
                    action: ComposerStore.actions.saveDraftAction
                    showLabel: row.textVisible
                }

                TitledHeaderButton {
                    height: units.gu(4)
                    anchors.verticalCenter: parent.verticalCenter
                    action: Action {
                        text: qsTr("Attach")
                        iconName: Icons.AttachmentIcon
                        iconSource: Paths.actionIconUrl(Icons.AttachmentIcon)
                        onTriggered: ContentActions.pickFile(composer)
                    }
                    showLabel: row.textVisible
                }

                Stretcher {}

                TitledHeaderButton {
                    height: units.gu(4)
                    anchors.verticalCenter: parent.verticalCenter
                    visible: ComposerStore.hasValidIdentity
                    action: ComposerStore.actions.sendAction
                    showLabel: row.textVisible
                }
            }

            Line {
                anchors {
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left
                }
            }
        }

        Stretcher {
            id: content

            StretchRow {
                anchors.fill: parent
                spacing: 0

                Stretcher {
                    implicitHeight: parent.height - attachmentPanel.height
                    MessageComposer {
                        id: composePanel
                        anchors.fill: parent
                    }
                }

                AttachmentPanel {
                    id: attachmentPanel
                    attachments: ComposerStore.attachments
                    isReadOnly: false
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                        right: parent.right
                    }
                    maxHeight: parent.height / 2
                }
            }
        }
    }
}
