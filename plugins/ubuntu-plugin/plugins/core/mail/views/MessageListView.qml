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
import QtQuick.Controls.Suru 2.2
import Lomiri.Components 1.3
import SageteaMail.Mail.Accounts 1.0
import SageteaMail.Mail 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Stores.Mail 1.0
import SageteaMail.Components 1.0
import SageteaMail.Mail.Settings 1.0
import SageteaMail.Lomiri.Components 1.0
import SageteaMail.Lomiri.Constants 1.0
import QuickFlux 1.0
import PlugMan 1.0
import MazDB 1.0
import "../delegates"
import "../models"
import "../composer"

DekkoPage {
    id: msgListPage

    property alias mboxSearch: loader_mboxSearch.item

    pageHeader {
        title: MailStore.folderTitle
        enableSearching: true
        composeButtonEnabled: !sageteamail.isLargeFF
        backAction: sageteamail.isSmallFF || MailStore.isInSelectionMode ? drawerAction : null
        filterSections: isSearchMode ? MailStore.searchFilters : MailStore.defaultFilters
        multiSelectActionList: multiSelectRegistry.actions
    }

    onSelectedIndexChanged: {
        if (isSearchMode) {
            if (selectedIndex === 0) {
                mboxSearch.location = MailboxSearch.Local
            } else {
                mboxSearch.location = MailboxSearch.Remote
            }
            return;
        }
        var filter = MessageList.All;
        switch (selectedIndex) {
        case 0: // All
            filter = MessageList.All;
            break;
        case 1: // Unread
            filter = MessageList.Unread;
            break;
        case 2: // Starred
            filter = MessageList.Important;
            break;
        case 3: // Replied
            filter = MessageList.Replied;
            break;
        case 4: // Forwarded
            filter = MessageList.Forwarded;
            break;
        case 5: // Attachments
            filter = MessageList.Attachments;
            break;
        case 6: // Calendar
            filter = MessageList.Calendar;
            break;
        }
        MessageActions.setMessageListFilter(filter)
    }

    onMultiSelectCanceled: MessageActions.endMultiSelection()
    onIsSearchModeChanged: {
        if (isSearchMode) {
            listView.currentIndex = -1
            listView.model = mboxSearch.results
        } else {
            MessageActions.resetMessageList()
        }
    }

    onSearchActivated: mboxSearch.search(searchString)
    onSearchCanceled: {
        pageHeader.searchInProgress = false;
        mboxSearch.cancelSearch()
    }

    MazDBSettings {
        category: "ui-property-cache"
        property alias msgListFilterIndex: msgListPage.selectedIndex
    }

    // Plugins can add bulk message actions by registering Action for this location
    ActionRegistry {
        id: multiSelectRegistry
        location: MailStore.listenerId ? "SageteaMail::Mail::MultiSelectAction" : ""
        defaultActions: [
            Action {
                iconSource: !MailStore.msgList.canSelectAll ? Paths.actionIconUrl(Paths.NoneSelectedIcon) :
                                                              Paths.actionIconUrl(Paths.SelectIcon)
                text: MailStore.msgList.canSelectAll ? qsTr("Unselect all") : qsTr("Select all")
                onTriggered: {
                    if (MailStore.msgList.canSelectAll) {
                        MessageActions.selectAllMessages()
                    } else {
                        MessageActions.unselectAllMessages()
                    }
                }
            },
            Action {
                iconSource: MailStore.msgList.canMarkSelectionImportant ? Paths.actionIconUrl(Paths.StarredIcon) :
                                                                Paths.actionIconUrl(Paths.UnStarredIcon)
                text: MailStore.msgList.canMarkSelectionImportant ? qsTr("Star") : qsTr("Remove star")
                onTriggered: MessageActions.markSelectedMessagesImportant()
            },
            Action {
                text: MailStore.msgList.canMarkSelectionRead ? qsTr("Mark as un-read") : qsTr("Mark as read")
                iconSource: !MailStore.msgList.canMarkSelectionRead ? Paths.actionIconUrl(Paths.MailUnreadIcon) :
                                                            Paths.actionIconUrl(Paths.MailReadIcon)
                onTriggered: MessageActions.markSelectedMessagesRead()
            },
            Action {
                iconSource: Paths.actionIconUrl(Paths.DeleteIcon)
                text: qsTr("Delete")
                onTriggered: MessageActions.deleteSelectedMessages()
            }
        ]
    }

    Action {
        id: drawerAction
        iconName: "navigation-menu"
        onTriggered: ViewActions.toggleNavDrawer()
    }

    Loader {
        id: loader_mboxSearch
        asynchronous: true
        sourceComponent: Component {
            id: component_mboxSearch
            MailboxSearch {
                messageKey: MailStore.msgListKey
                location: MailboxSearch.Local
                limit: 50
                sortBy: MailboxSearch.TimeStamp
                sortOrder: Qt.DescendingOrder
                onStatusChanged: {
                    if (status === SearchService.InProgress) {
                        pageHeader.searchInProgress = true
                    } else {
                        pageHeader.searchInProgress = false
                    }
                }
            }
        }
    }


    PageContent {
        id: cont

        ListItem {
            id: undoNotification
            expansion.height: units.gu(6)
            expansion.expanded: MailStore.hasUndoableActions
            divider.visible: MailStore.hasUndoableActions
            height: 0
            anchors {
                left: parent.left
                top: parent.top
                right: parent.right
            }
            color: Suru.secondaryBackgroundColor

            Label {
                anchors {
                    left: parent.left
                    leftMargin: Style.defaultSpacing
                    verticalCenter: parent.verticalCenter
                    right: undoButton.left
                    rightMargin: units.gu(1.5)
                }
                elide: Text.ElideRight
                clip: true
                visible: MailStore.hasUndoableActions
                text: MailStore.undoableActionDescription
            }

            Button {
                id: undoButton
                height: Style.largeSpacing
                width: undoAction.text.width > units.gu(15) ? units.gu(15) : undoAction.text.width
                anchors {
                    right: parent.right
                    rightMargin: Style.defaultSpacing
                    verticalCenter: parent.verticalCenter
                }
                Suru.highlightType: Suru.PositiveHighlight
                color: Suru.highlightColor
                action: Action {
                    id: undoAction
                    text: qsTr("Undo")
                    onTriggered: MessageActions.undoRecentActions()
                }
            }
        }

        ScrollView {

            anchors {
                left: parent.left
                top: undoNotification.bottom
                right: parent.right
                bottom: parent.bottom
            }

            Keys.forwardTo: [listView]

            ListView {
                id: listView
                property int selectionIndex: 0
                // Workaround to stop the message at index 0 being loaded when
                // switching back from search mode.
                property bool dontLoadNextCurrentItem: false

                anchors.fill: parent
                clip: true
                currentIndex: -1
                onCurrentIndexChanged: {
                    MailStore.currentSelectedIndex  = currentIndex
                }

                onCurrentItemChanged: {
                    if (!currentItem || currentIndex === -1) {
                        return
                    }
                    if (dontLoadNextCurrentItem) {
                        currentIndex = -1
                        dontLoadNextCurrentItem = false
                        return
                    }

                    MessageActions.openMessage(currentItem.msg.messageId)
                    if (currentIndex == MailStore.msgListModel.count - 1) {
                        MessageActions.showMoreMessages()
                    }
                }

//                add: DekkoAnimation.listViewAddTransition
                addDisplaced: DekkoAnimation.listViewAddDisplacedTransition
                remove: DekkoAnimation.listViewRemoveTransition
                removeDisplaced: DekkoAnimation.listViewRemoveDisplacedTransition

                highlight: !sageteamail.isSmallFF ? Style.highlightBar : null
                highlightFollowsCurrentItem: true
                highlightMoveDuration: 200
                cacheBuffer: units.gu(120)
                model: MailStore.msgListModel
                delegate: MessageListDelegate{
                    id: msgListDelegate
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    // This prevents making excessive calls to data()
                    // We just take the actual MinimalMessage object
                    // in one go and reuse it throughout.
                    msg: model.qtObject

                    leftSideAction: Action {
                        iconName: "delete"
                        onTriggered: {
                            MessageActions.deleteMessage(msgListDelegate.msg.messageId)
                        }
                    }

                    rightSideActions: [flagAction, readAction, contextAction]

                    onItemClicked: {

                        if (MailStore.isInSelectionMode) {
                            if (msg.checked) {
                                MessageActions.setMessageCheck(model.index, Qt.Unchecked)
                            } else {
                                MessageActions.setMessageCheck(model.index, Qt.Checked)
                            }
                            return;
                        }

                        if (msg.isDraft) {
                            ComposerActions.openDraft(msg.messageId)
                            return;
                        }

                        if (mouse.button === Qt.RightButton) {
                            rightClickActions.trigger()
                            return;
                        }

                        listView.currentIndex = model.index
                    }
                    onItemPressAndHold: {
                        // TODO: get multiselect working on search results.
                        if (isSearchMode) {
                            return;
                        }
                        if (!MailStore.isInSelectionMode) {
                            listView.selectionIndex = model.index
                            MessageActions.startMultiSelection()
                            MessageActions.setMessageCheck(model.index, Qt.Checked)
                            msgListPage.startMultiSelect()
                        }
                    }
                }
                // canLoadMore will always be false when the list is empty so there
                // isn't any need to add a binding on count/totalCount as that signal
                // emits *very* often and really bogs down the UI. canPossiblyLoadMore will
                // only get emitted after all the insertions/removals have completed for a query.
                footer: (!isSearchMode && !MailStore.msgListLoading && MailStore.canLoadMore) ? footerComponent : null

                Component {
                    id: footerComponent
                    ListItem {
                        Label {
                            anchors.centerIn: parent
                            text: qsTr("Load more messages ...")
                        }
                        onClicked: MessageActions.showMoreMessages()
                    }
                }

                PullToRefresh {
                    refreshing: MailboxStore.folderSyncActive
                    onRefresh: MailboxActions.syncStandardFolders(Folder.StandardFolder)
                }
            }
        }

        ActivityIndicator {
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
                topMargin: units.gu(20)
            }
            visible: MailStore.msgListLoading
            running: MailStore.msgListLoading
        }
    }

    Binding {
        target: listView;
        property: "currentIndex";
        value: MailStore.currentSelectedIndex
        when: MailStore
    }

    AppListener {

        Filter {
            type: MessageKeys.resetMessageList
            onDispatched: {
                listView.currentIndex = -1
                listView.positionViewAtBeginning()
                selectedIndex = 0
                if (listView.model !== MailStore.msgListModel) {
                    listView.dontLoadNextCurrentItem = true
                    listView.model = MailStore.msgListModel
                }
            }
        }

        Filter {
            type: MailboxKeys.resetSelectedMsgIndex
            onDispatched: {
                listView.currentIndex = -1
            }
        }
    }
}
