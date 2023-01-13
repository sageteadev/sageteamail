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
import Ubuntu.Components 1.3
import SageteaMail.Components 1.0
import SageteaMail.Ubuntu.Components 1.0
import SageteaMail.Ubuntu.Helpers 1.0

StretchColumn {
    height: parent.height
    width: parent.width
    default property alias content: mainContainer.data
    property alias panelEnabled: pc.visible
    property alias viewState: pc.viewState
    // We want the main content to take up as much space as possible
    // So use the Stretcher to get this. We are going to add a tabbed
    // panel below this at some point to show log output etc
    Stretcher {
        id: mainContainer
        anchors {
            left: parent.left
            right: parent.right
        }
    }

    PanelContainer {
        id: pc
        width: parent.width
        visible: false
        activeEdge: Item.Top
        Loader {
            id: panelLoader
            anchors.fill: parent
            active: pc.visible
            asynchronous: true
            sourceComponent: Component {
                DevPanel {
                    id: panel
                    anchors.fill: parent
                }
            }
        }
    }
}

