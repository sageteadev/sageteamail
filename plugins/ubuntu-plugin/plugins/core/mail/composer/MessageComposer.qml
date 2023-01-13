/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>
   Copyright (C) 2022 - 2023 Rúben Carneiro <rubencarneiro01@gmail.com>

   This file is part of SageTea Mail, email client for Ubuntu devices

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
import QtQuick 2.4
import QtQuick.Controls.Suru 2.2
import Ubuntu.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Composer 1.0
import SageteaMail.Markdown 1.0
import SageteaMail.Ubuntu.Constants 1.0

FocusScope {
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: Suru.backgroundColor
    }

    MarkdownPartBuilder {
        id: pbuilder
        onPartsReady: ContentActions.displayHtmlContent(composer, parts.html)
    }

    Binding {
        target: ComposerStore
        property: "bodyDocument"
        value: me.textDocument
    }

    MarkdownEditor {
        id: me
        anchors.fill: parent
        frameSpacing: units.gu(2)
        plainTextOnly: true

        onShowPreview: {
            var sig = ComposerStore.currentSignature ? "--  \n%1".arg(ComposerStore.currentSignature) : ""
            pbuilder.build(text, sig)
        }

        header: Column {
            id: contentHeader
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            SenderIdentityField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            RecipientField {
                recipientType: RecipientType.To
                recipientModel: ComposerStore.recipients.to
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }

            RecipientField {
                visible: ComposerStore.showCC || ComposerStore.recipients.cc.count
                recipientType: RecipientType.Cc
                recipientModel: ComposerStore.recipients.cc
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            RecipientField {
                visible: ComposerStore.showBCC || ComposerStore.recipients.bcc.count
                recipientType: RecipientType.Bcc
                recipientModel: ComposerStore.recipients.bcc
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
            SubjectField {
                anchors {
                    left: parent.left
                    right: parent.right
                }
            }
        }
    }
}

