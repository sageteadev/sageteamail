import QtQuick 2.4
import Lomiri.Components 1.3
import SageteaMail.Mail 1.0
import SageteaMail.Mail.Accounts 1.0
import SageteaMail.Mail.API 1.0
import SageteaMail.Mail.Settings 1.0
import SageteaMail.Mail.Stores.Accounts 1.0
import SageteaMail.Mail.Stores.Mail 1.0
import SageteaMail.Mail.Stores.Views 1.0
import SageteaMail.Components 1.0
import MazDB 1.0
import PlugMan 1.0
import SageteaMail.Lomiri.Components 1.0
import "../delegates"

StyledItem {
    width: parent.width
    height: parent.height

    property bool panelMode: false

    theme: ThemeSettings{
        name: "Lomiri.Components.Themes.SuruDark"
    }

    MazDBSettings {
        category: "ui-property-cache"
        property alias smartFoldersExpanded: smf.expanded
        property alias accountFoldersExpanded: acg.expanded
    }

    Rectangle {
        anchors.fill: parent
        color: LomiriColors.inkstone
    }

    ActionRegistry {
        id: headerRegistry
        location: "SageteaMail::Mail::NavHeadAction"
        defaultActions: [
            Action {
                iconName: "edit"
                onTriggered: ViewActions.openComposer()
                visible: sageteamail.isLargeFF
            },
            Action {
                iconName: "info"
                visible: sageteamail.isSmallFF
                onTriggered: ViewActions.openSettings()
            },
            Action {
                iconName: "settings"
                visible: sageteamail.isSmallFF
                onTriggered: ViewActions.openSettings()
            }
        ]
    }

    StretchColumn {
        anchors {
            fill: parent
        }

        Item {
            width: parent.width
            height: units.gu(5)
            implicitHeight: height
            visible: !sageteamail.isMediumFF

            StretchRow {
                anchors {
                    leftMargin: units.gu(1)
                    fill: parent
                    rightMargin: units.gu(1)
                }

                HeaderButton {
                    height: units.gu(5.5)
                    width: units.gu(5)
                    implicitWidth: width
                    iconColor: LomiriColors.silk
                    iconSize: units.gu(2.5)
                    highlightColor: LomiriColors.slate
                    action: Action {
                        visible: sageteamail.isSmallFF
                        iconName: "navigation-menu"
                        onTriggered: ViewActions.toggleNavDrawer()
                    }
                }

                Stretcher {}

                Repeater {
                    model: headerRegistry.actions
                    delegate: HeaderButton {
                        height: units.gu(5.5)
                        width: units.gu(5)
                        implicitWidth: width
                        iconColor: LomiriColors.silk
                        iconSize: units.gu(2.5)
                        highlightColor: LomiriColors.slate
                        action: modelData
                    }
                }
            }
        }

        Stretcher {
            anchors {
                left: parent.left
                right: parent.right
            }

            ScrollView {
                anchors.fill: parent
                Flickable {
                    anchors.fill: parent
                    contentHeight: col.height

                    Column {
                        id: col
                        anchors {
                            left: parent.left
                            right: parent.right
                            top: parent.top
                        }

                        Repeater {
                            id: inboxList
                            model: MailboxStore.standardFoldersModel
                            delegate: NavMenuStandardFolderDelegate {
                                id: folderDelegate
                                folder: qtObject
                                supportsDescendents: true
                                onClicked: {
                                    if (model.index === 0) {
                                        MessageActions.openFolder(folder.displayName, folder.descendentsKey)
                                    } else {
                                        MessageActions.openFolder(folder.displayName, folder.messageKey)
                                    }
                                }
                                onSubFolderClicked: MessageActions.openFolder(name, key)
                                Component.onCompleted: {
                                    if (model.index === 0 && !panelMode) {
                                        ViewActions.delayCallWithArgs(MessageKeys.openFolder, {
                                                                          folderName: folder.displayName,
                                                                          folderKey: folder.descendentsKey
                                                                      })
                                    }
                                }
                            }
                        }

                        NavigationGroup {
                            id: smf
                            title: qsTr("Smart folders")
                            model: MailboxStore.smartFoldersModel
//                            expansion.expanded: PolicyManager.views.smartFoldersExpanded
//                            onExpandClicked: PolicyManager.views.smartFoldersExpanded = !PolicyManager.views.smartFoldersExpanded
                            delegate: SmartFolderDelegate {
                                id: smartFolderDelegate
                                folder: qtObject
                                smartFolder: true
                                onClicked: MessageActions.openFolder(folder.displayName, folder.messageKey)
                            }
                        }

                        NavigationGroup {
                            id: acg
                            title: qsTr("Folders")
                            model: AccountStore.receiveAccountsModel
//                            expansion.expanded: PolicyManager.views.accountsExpanded
//                            onExpandClicked: PolicyManager.views.accountsExpanded = !PolicyManager.views.accountsExpanded
                            delegate: ListItem {
                                height: dLayout.height
                                divider.visible: false
                                onClicked: MessageActions.openAccountFolder(qtObject.name, qtObject.id)
                                Rectangle {
                                    anchors.fill: parent
                                    color: Qt.rgba(0, 0, 0, 0.05)
                                    visible: dLayout ? dLayout.title.text === ViewStore.selectedNavFolder : false
                                }
                                ListItemLayout {
                                    id: dLayout
                                    height: units.gu(6)
                                    title.text: qtObject.name
                                    Icon {
                                        height: units.gu(2.5)
                                        width: height
                                        name: "contact"
                                        SlotsLayout.position: SlotsLayout.Leading
                                    }
                                    ProgressionSlot{}
                                }
                            }
                        }

                        Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        width: parent.width * 4 / 5
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        text: i18n.tr("You are runnig a free edition of SageTeaMail, consider upgrade to Pro")
                        font.bold: true
                        onLinkActivated: Qt.openUrlExternally(link)
                    }

                        Rectangle {
                        color: "transparent"
                        width: parent.width
                        height: units.gu(3)
                    }

                        Button {
                        id: upgradeButton
                        
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: LomiriColors.orange
                        text: i18n.tr("Upgrade Pro")
                        onClicked: Qt.openUrlExternally("https://sagetea.ai/product/sagetea-mail/")
                    }

//                        Item {
//                            anchors {
//                                left: parent.left
//                                right: parent.right
//                            }
//                            height: lbl.height + units.gu(2)
//                            implicitHeight: height
//                            Label {
//                                id: lbl
//                                anchors {
//                                    left: parent.left
//                                    leftMargin: units.gu(1)
//                                    right: parent.right
//                                    verticalCenter: parent.verticalCenter
//                                }
//                                text: "Smart Folders"
//                                color: LomiriColors.ash
//                            }
//                        }
//                        Line {
//                            anchors {
//                                left: parent.left
//                                leftMargin: units.gu(1)
//                                right: parent.right
//                                rightMargin: units.gu(1)
//                            }
//                            color: LomiriColors.slate
//                        }


//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Today"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "calendar-today"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Todo"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "view-list-symbolic"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Done"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "tick"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }

//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Mailing Lists"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "contact-group"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Search: \"dekko-dev\""
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "search"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }

//                        Item {
//                            anchors {
//                                left: parent.left
//                                right: parent.right
//                            }
//                            height: lbl2.height + units.gu(2)
//                            implicitHeight: height
//                            Label {
//                                id: lbl2
//                                anchors {
//                                    left: parent.left
//                                    leftMargin: units.gu(1)
//                                    right: parent.right
//                                    verticalCenter: parent.verticalCenter
//                                }
//                                text: "Folders"
//                                color: LomiriColors.ash
//                            }
//                        }

//                        Line {
//                            anchors {
//                                left: parent.left
//                                leftMargin: units.gu(1)
//                                right: parent.right
//                                rightMargin: units.gu(1)
//                            }
//                            color: LomiriColors.slate
//                        }

//                        Item {
//                            height: tf.height + units.gu(2)
//                            implicitHeight: height
//                            width: parent.width
//                            TextField {
//                                id: tf
//                                anchors {
//                                    left: parent.left
//                                    leftMargin: units.gu(2)
//                                    right: parent.right
//                                    rightMargin: units.gu(2)
//                                    verticalCenter: parent.verticalCenter
//                                }
//                                placeholderText: "Search folders"
//                            }
//                        }

//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "GMail"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "contact"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Dekko"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "contact"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
//                        ListItem {
//                            implicitHeight: i1.implicitHeight
//                            divider.visible: false
//                            highlightColor: LomiriColors.ash
//                            ListItemLayout {
//                                title.text: "Home"
//                                title.font.weight: Font.DemiBold
//                                title.color: LomiriColors.silk
//                                Icon {
//                                    name: "contact"
//                                    color: LomiriColors.silk
//                                    height: units.gu(2.2)
//                                    width: height

//                                    SlotsLayout.position: SlotsLayout.Leading
//                                }
//                            }
//                            onClicked: console.log("Clicked")
//                        }
                    }
                }
            }
        }
    }
}
