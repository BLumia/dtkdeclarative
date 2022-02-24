/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Templates 2.4 as T
import QtQuick.Layouts 1.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.CheckBox {
    id: control

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)
    baselineOffset: contentItem.y + contentItem.baselineOffset
    icon {
        width: DS.Style.checkBox.iconSize
        height: DS.Style.checkBox.iconSize
    }
    checkable: true
    font: D.DTK.fontManager.t8
    padding: DS.Style.checkBox.padding
    spacing: DS.Style.control.spacing
    opacity: D.ColorSelector.controlState === D.DTK.DisabledState ? 0.4 : 1
    D.DciIcon.mode: D.ColorSelector.controlState
    indicator: Item {
        x: control.text ? (!control.mirrored ? control.leftPadding : control.width - width - control.rightPadding) : control.leftPadding + (control.availableWidth - width) / 2
        y: control.topPadding + (control.availableHeight - height) / 2
        implicitWidth: DS.Style.checkBox.indicatorWidth
        implicitHeight: DS.Style.checkBox.indicatorHeight
        D.DciIcon {
            palette: control.D.DTK.makeIconPalette(control.palette)
            mode: control.D.ColorSelector.controlState
            name: control.checkState === Qt.Checked ? "checkbox_checked" : control.checkState === Qt.Unchecked ? "checkbox_unchecked" : "checkbox_mix"
            sourceSize: Qt.size(control.icon.width, control.icon.height)
        }

        FocusBoxBorder {
            visible: control.activeFocus
            color: control.palette.highlight
            radius: DS.Style.checkBox.focusRadius
            anchors.fill: parent
        }
    }

    contentItem: Text {
        leftPadding: control.indicator && !control.mirrored ? control.indicator.width + control.spacing : 0
        rightPadding: control.indicator && control.mirrored ? control.indicator.width + control.spacing : 0
        text: control.text
        font: control.font
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
    }

    background: null
}
