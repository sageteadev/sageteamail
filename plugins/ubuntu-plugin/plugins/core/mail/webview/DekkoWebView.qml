/* Copyright (C) 2014-2016 Dan Chapman <dpniel@ubuntu.com>

   This file is part of Dekko email client for Ubuntu Devices/

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
import QtFeedback 5.0
import Lomiri.Components 1.3
import Lomiri.Components.Popups 1.3
import Lomiri.Components.ListItems 1.3 as ListItem
import Lomiri.Content 1.3
import QtQuick.Controls 2.1 as QQC2
import QtWebEngine 1.5
import SageteaMail.Components 1.0
import SageteaMail.Mail.Settings 1.0
import SageteaMail.Lomiri.Components 1.0
import SageteaMail.Lomiri.Dialogs 1.0
import SageteaMail.WebContext 1.0
import QtQuick.Controls.Suru 2.2

WebEngineView {
    id: webview

    property bool fullScreen: false
    property int messageUid
    readonly property alias contentBlocked: internal.remoteContentBlockRunning
    property alias context: webview.profile 

    zoomFactor: units.gu(1) / 8

    function setCidQuery(uid) {
        messageUid = uid;
    }

    function setBodyUrl(body) {
        webview.url = body
    }

    function allowBlockedContent() {
        internal.remoteContentBlockRunning = false
        ctxt.remoteContentAllowed = true;
        reload()
    }

    // LOCK IT DOWN!!
    // incognito: true can't set his as it disables userscripts.
    settings {
        // We should NEVER allow javascript to run in
        // a message body. See https://miki.it/blog/2013/9/24/mailboxapp-javascript-execution/ for
        // how mailbox (by dropbox) got it wrong :-)
        // http://doc.qt.io/qt-5/qml-qtwebengine-webenginesettings.html
        javascriptEnabled: false
        javascriptCanAccessClipboard: false
        localContentCanAccessFileUrls: false
        autoLoadImages: true
        localStorageEnabled: false
        hyperlinkAuditingEnabled: false
        defaultTextEncoding: "UTF-8"
    }

    userScripts: [
        WebEngineScript {
            sourceUrl: Qt.resolvedUrl("./userscripts/font_user_script.js")
            runOnSubframes: true
        },
        WebEngineScript {
            sourceUrl: Qt.resolvedUrl("./userscripts/viewport_shim.js")
            runOnSubframes: true
        }
    ]

    context: SageteaMailWebContext {
        id: ctxt

        readonly property string defaultUserAgent: _ua.defaultUA

        qmlEngineInjector: parent

        httpUserAgent: defaultUserAgent
        httpCacheMaximumSize: cacheSizeHint
        cachePath: cacheLocation
        persistentStoragePath: dataLocation

        persistentCookiesPolicy: WebEngineProfile.NoPersistentCookies

        messageUid: webview.messageUid
        remoteContentAllowed: PolicyManager.privacy.allowRemoteContent

        onRemoteContentBlocked: {
            if (!internal.remoteContentBlockRunning) {
                internal.remoteContentBlockRunning = true
            }
        }

        property QtObject _ua: QtObject {
            id: defaultUA
            // Empirical value: screens smaller than 19cm are considered small enough that a
            // mobile UA string is used, screens bigger than that will get desktop content.
            readonly property bool smallScreen: ctxt.screenDiagonal < 190

            // %1: Ubuntu version, e.g. "14.04"
            // %2: optional token to specify further attributes of the platform, e.g. "like Android"
            // %3: optional hardware ID token
            // %4: WebKit version, e.g. "537.36"
            // %5: Chromium version, e.g. "35.0.1870.2"
            // %6: Optional token to provide additional free-form information, e.g. "Mobile"
            // %7: Safari version, e.g. "537.36"
            // %8: Optional token, in case some extra bits are needed to make things work (e.g. an extra form factor info etc.)
            //
            // note #1: "Mozilla/5.0" is misinformation, but it is a legacy token that
            //   virtually every single UA out there has, it seems unwise to remove it
            // note #2: "AppleWebKit", as opposed to plain "WebKit", does make a
            //   difference in the content served by certain sites (e.g. gmail.com)
            readonly property string _template: "Mozilla/5.0 (Linux; Ubuntu %1%2%3) AppleWebKit/%4 Chromium/%5 %6Safari/%7%8"

            // FIXME: compute at build time (using lsb_release)
            readonly property string _ubuntuVersion: "20.04"

            readonly property string _attributes: smallScreen ? "like Android" : ""

            readonly property string _hardwareID: ""

            // See chromium/src/content/webkit_version.h.in in oxide’s source tree.
            readonly property string _webkitVersion: "537.36"

            // See chromium/src/chrome/VERSION in oxide’s source tree.
            // Note: the actual version number probably doesn’t matter that much,
            //       however its format does, so we probably don’t need to bump it
            //       every time we rebase on a newer chromium.
            readonly property string _chromiumVersion: "35.0.1870.2"

            readonly property string _formFactor: smallScreen ? "Mobile" : ""

            readonly property string _more: ""

            property string defaultUA: {
                var ua = _template
                ua = ua.arg(_ubuntuVersion) // %1
                ua = ua.arg((_attributes !== "") ? " %1".arg(_attributes) : "") // %2
                ua = ua.arg((_hardwareID !== "") ? "; %1".arg(_hardwareID) : "") // %3
                ua = ua.arg(_webkitVersion) // %4
                ua = ua.arg(_chromiumVersion) // %5
                ua = ua.arg((_formFactor !== "") ? "%1 ".arg(_formFactor) : "") // %6
                ua = ua.arg(_webkitVersion) // %7
                ua = ua.arg((_more !== "") ? " %1".arg(_more) : "") // %8
                return ua
            }
        }
    }

    // Binding the contxt popover directly to this causes
    // it to get rendered below message header.
    // So instead just use this item to grab the model context property
    // And popup the dialog outside of this binding.
    //

    MimeData {
        id: mimeData
        text: contextContentText
    }

    property string contextContentText: ""
    property bool contextContentIsUrl: false
    property bool contextContentIsMail: contextContentText.startsWith("mailto:")
    onContextMenuRequested: {
        request.accepted = true // The request is handled here
        if (request.selectedText !== "") {
            contextContentText = request.selectedText
            contextContentIsUrl = false
            contextMenuComponent.open()
    }
        else if (request.linkText !== "") {
            contextContentText = request.linkUrl
            contextContentIsUrl = true
            contextMenuComponent.open()
        }
	}


    QQC2.Dialog {
        id: contextMenuComponent

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        ActionBar {
            numberOfSlots: 5
            actions: [
                Action {
                    iconName: contextContentIsMail ? "mail-unread" : "external-link"
                    text: contextContentIsMail ? i18n.tr("New mail") : i18n.tr("Open link")
                    enabled: contextContentIsUrl
                    onTriggered: {
                        Qt.openUrlExternally(contextContentText)
                        contextMenuComponent.close()
                    }
                },
                Action {
                    iconName: "edit-select-all"
                    text: i18n.tr("Select all")
                    onTriggered: webview.triggerWebAction(WebEngineView.SelectAll)
                },
                Action {
                    iconName: "edit-copy"
                    text: i18n.tr("Copy")
                    onTriggered: {
                        Clipboard.push( mimeData )
                        contextMenuComponent.close()
                    }
                }
            ]
            delegate:  Rectangle {
                color: mousearea.pressed ? Suru.secondaryBackgroundColor : Suru.backgroundColor
                width: units.gu(7)
                height: Math.max(units.gu(4), implicitWidth) + units.gu(1)
                visible: modelData.enabled
                Icon {
                    width: units.gu(2.5)
                    height: width
                    name: modelData.iconName
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Label {
                    text: modelData.text
                    textSize: Label.XSmall
                    width: parent.width - units.gu(3)
                    wrapMode: Text.WrapWord
                    horizontalAlignment: Text.AlignHCenter
                    anchors.bottom: parent.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                MouseArea {
                    id: mousearea
                    anchors.fill: parent
                    onClicked: modelData.triggered(null)
                }
            }
        }
        }

    onNewViewRequested: navigationRequested

    onNavigationRequested: {
        console.log("[SageteaMail Web View] RequestUrl", request.url)
        if (("" + request.url).startsWith("dekko-msg") ||
            ("" + request.url).startsWith("dekko-part") ||
            ("" + request.url).startsWith("cid")) {
            request.action = 0
        }
        else {
            PopupUtils.open(linkClickedDialog, sageteamail, {externalLink: request.url})
            request.action = 255
        }
    }

    onJavaScriptConsoleMessage: {
        var msg = "[SageteaMail Web View] [JS] (%1:%2) %3".arg(sourceID).arg(lineNumber).arg(message)
        console.log(msg)
    }

    Item {
        id: contextualRectangle
        visible: true
        readonly property real locationBarOffset: _webview.locationBarController.height + _webview.locationBarController.offset
        x: internal.contextModel ? internal.contextModel.position.x : 0
        y: internal.contextModel ? internal.contextModel.position.y + locationBarOffset : 0
    }

    QtObject {
        id: internal
        property QtObject ctxtModel: null
        property bool remoteContentBlockRunning: false
    }

    HapticsEffect {
        id: clickEffect
        attackIntensity: 0.0
        attackTime: 50
        intensity: 1.0
        duration: 10
        fadeTime: 50
        fadeIntensity: 0.0
    }

    Component {
        id: linkClickedDialog
        ConfirmationDialog {
            id: linkDialog
            property string externalLink
            title: qsTr("Open in browser?")
            text: qsTr("Confirm to open %1 in web browser").arg(externalLink.substring(0, 30))
            onConfirmClicked: Qt.openUrlExternally(externalLink)
        }
    }
}

