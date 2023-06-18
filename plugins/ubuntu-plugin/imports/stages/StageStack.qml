/* Copyright (C) 2016 - 2017 Dan Chapman <dpniel@ubuntu.com>

   This file is part of Dekko email client for Ubuntu devices

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
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import QuickFlux 1.0
import SageteaMail.Controls 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Views 1.0
import SageteaMail.Lomiri.Constants 1.0

// {item: message.page, properties: message.properties}
StackView {
    id: stageStack

    delegate: DekkoAnimation.customStackViewDelegate2

    AppListener {
        waitFor: ViewStore.listenerId
        Filter {
            type: ViewKeys.pushStage
            onDispatched: {
                Log.logStatus("StageStack::pushStage", "Pushing stage: " + message.stage)
                stageStack.push({item: Qt.resolvedUrl(message.stage), properties: message.properties})
            }
        }
        Filter {
            type: ViewKeys.popStage
            onDispatched: {
                Log.logStatus("StageStack::popStage", "Popping top stage: ")
                stageStack.pop()
            }
        }
        Filter {
            type: ViewKeys.clearStageStack
            onDispatched: {
                Log.logStatus("StageStack::clearStageStack", "Clearing stagestack ")
                stageStack.clear()
            }
        }
        Filter {
            type: ViewKeys.openMessageComposer
            onDispatched: {
                if (sageteamail.isLargeFF) {
                    PopupUtils.open(Qt.resolvedUrl("../composer/ComposePanelPopup.qml"), dekkoContainer, {})
                }
            }
        }
        Filter {
            type: ViewKeys.openAddressBook
            onDispatched: {
                Log.logStatus("StageStack::openAddressBook", "Opening addressbook stage ")
                ViewActions.pushStage("./AddressBookStage.qml", {})
            }
        }

        Filter {
            type: ViewKeys.rewindStageStack
            onDispatched: {
                Log.logStatus("StageStack::rewindStageStack", "Rewinding stage stack ")
                while (stageStack.depth > 1) {
                    stageStack.pop()
                }
            }
        }
    }
}

