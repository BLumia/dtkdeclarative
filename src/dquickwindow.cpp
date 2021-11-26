/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <DPlatformHandle>

#include "dquickwindow.h"
#include "private/dquickwindow_p.h"

DQUICK_BEGIN_NAMESPACE

DQuickWindowPrivate::DQuickWindowPrivate(DQuickWindow *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
    , attached(new DQuickWindowAttached(qq))
{
}

DQuickWindow::DQuickWindow(QWindow *parent)
    : QQuickWindow(parent)
    , DObject(*new DQuickWindowPrivate(this))
{

}

DQuickWindow::~DQuickWindow()
{
}

/*!
 * \~chinese \brief DQuickWindow::windowAttached　用于获取窗口的附加属性对象，
 * \~chinese 可以设置通过这个对象设置窗口的圆角、边框等属性值。
 */
DQuickWindowAttached *DQuickWindow::attached() const
{
    D_DC(DQuickWindow);

    return d->attached;
}

/*!
 * \~chinese \brief DQuickWindow::qmlAttachedProperties 用于创建窗口的附加属性对象，
 * \~chinese 在 QML 中使用附加属性时，会自动调用此函数。
 */
DQuickWindowAttached *DQuickWindow::qmlAttachedProperties(QObject *object)
{
    QQuickWindow *window = qobject_cast<QQuickWindow *>(object);
    if (window) {
        return new DQuickWindowAttached(window);
    }

    return nullptr;
}

DQuickWindowAttachedPrivate::DQuickWindowAttachedPrivate(DQuickWindowAttached *qq)
    : DObjectPrivate(qq)
    , wmWindowTypes(DWindowManagerHelper::UnknowWindowType)
    , explicitEnable(false)
{

}

DQuickWindowAttachedPrivate::~DQuickWindowAttachedPrivate()
{
    if (handle) {
        delete handle;
    }
}

void DQuickWindowAttachedPrivate::updatePlatformHandle()
{
    if (static_cast<bool>(handle) == explicitEnable)
        return;

    D_Q(DQuickWindowAttached);

    auto tWindow = qobject_cast<QQuickWindow *>(q->parent());

    if (!explicitEnable) {
        handle->setEnabledNoTitlebarForWindow(tWindow, false);
        delete handle;
        handle = nullptr;
    } else if (DPlatformHandle::setEnabledNoTitlebarForWindow(tWindow, true)) {
        Q_ASSERT(DPlatformHandle::isEnabledNoTitlebar(tWindow));
        handle = new DPlatformHandle(tWindow);
        QObject::connect(handle, &DPlatformHandle::borderColorChanged, q, &DQuickWindowAttached::borderColorChanged);
        QObject::connect(handle, &DPlatformHandle::borderWidthChanged, q, &DQuickWindowAttached::borderWidthChanged);
        QObject::connect(handle, &DPlatformHandle::shadowColorChanged, q, &DQuickWindowAttached::shadowColorChanged);
        QObject::connect(handle, &DPlatformHandle::shadowOffsetChanged, q, &DQuickWindowAttached::shadowOffsetChanged);
        QObject::connect(handle, &DPlatformHandle::shadowRadiusChanged, q, &DQuickWindowAttached::shadowRadiusChanged);
        QObject::connect(handle, &DPlatformHandle::windowRadiusChanged, q, &DQuickWindowAttached::windowRadiusChanged);
        QObject::connect(handle, &DPlatformHandle::translucentBackgroundChanged, q, &DQuickWindowAttached::translucentBackgroundChanged);
        QObject::connect(handle, &DPlatformHandle::enableSystemMoveChanged, q, &DQuickWindowAttached::enableSystemMoveChanged);
        QObject::connect(handle, &DPlatformHandle::enableSystemResizeChanged, q, &DQuickWindowAttached::enableSystemResizeChanged);
        QObject::connect(handle, &DPlatformHandle::enableBlurWindowChanged, q, &DQuickWindowAttached::enableBlurWindowChanged);
    }
}

DQuickWindowAttached::DQuickWindowAttached(QWindow *window)
    : QObject(window)
    , DObject(*new DQuickWindowAttachedPrivate(this))
{
}

QQuickWindow *DQuickWindowAttached::window() const
{
    return qobject_cast<QQuickWindow *>(parent());
}

/*!
 * \~chinese \property DQuickWindowAttached::isEnabled
 * \~chinese \brief 这个属性用于判定是否使用了 DTK 风格的窗口。
 */
bool DQuickWindowAttached::isEnabled() const
{
    D_DC(DQuickWindowAttached);

    return d->explicitEnable && DPlatformHandle::isEnabledDXcb(window());
}

/*!
 * \~chinese \property DQuickWindowAttached::windowRadius
 * \~chinese \brief 这个属性保存着窗口圆角值。
 */
int DQuickWindowAttached::windowRadius() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return 0;
    }

    return d->handle->windowRadius();
}

/*!
 * \~chinese \property DQuickWindowAttached::borderWidth
 * \~chinese \brief 这个属性保存着窗口边框的宽度。
 */
int DQuickWindowAttached::borderWidth() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return 0;
    }

    return d->handle->borderWidth();
}

/*!
 * \~chinese \property DQuickWindowAttached::borderColor
 * \~chinese \brief 这个属性保存这窗口边框的颜色。
 */
QColor DQuickWindowAttached::borderColor() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->borderColor();
}

/*!
 * \~chinese \property DQuickWindowAttached::shadowRadius
 * \~chinese \brief 这个属性保存着窗口阴影半径。
 */
int DQuickWindowAttached::shadowRadius() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return 0;
    }

    return d->handle->shadowRadius();
}

/*!
 * \~chinese \property DQuickWindowAttached::shadowOffset
 * \~chinese \brief 这个属性保存着窗口阴影偏移量。
 */
QPoint DQuickWindowAttached::shadowOffset() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return QPoint();
    }

    return d->handle->shadowOffset();
}

/*!
 * \~chinese \property DQuickWindowAttached::shadowColor
 * \~chinese \brief 这个属性保存着窗口阴影颜色。
 */
QColor DQuickWindowAttached::shadowColor() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->shadowColor();
}

/*!
 * \~chinese \property DQuickWindowAttached::frameMask
 * \~chinese \brief 设置 Frame Window 的遮罩，和 \a clipPath 不同的是，它的裁剪包括阴影部分。
 * \~chinese \note 由于实现机制限制，使用此属性裁剪 Frame Window 时，无法去除边缘产生的锯齿。
 */
QRegion DQuickWindowAttached::frameMask() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return QRegion();
    }

    return d->handle->frameMask();
}

/*!
 * \~chinese \property DQuickWindowAttached::translucentBackground
 * \~chinese \brief 如果此属性值为 true，则在更新窗口绘制内容之前会先清空要更新区域内的图像，否则不清空。
 */
bool DQuickWindowAttached::translucentBackground() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return false;
    }

    return d->handle->translucentBackground();
}

/*!
 * \~chinese \brief DQuickWindowAttached::enableSystemResize
 * \~chinese \return 如果此属性值为 true，则允许外界改变窗口的大小（如使用鼠标拖拽窗口边框），否则不允许。
 * \~chinese \note 此属性仅仅控制 dxcb 中的行为，不会影响窗口管理器的行为。
 */
bool DQuickWindowAttached::enableSystemResize() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

/*!
 * \~chinese \property DQuickWindowAttached::enableSystemMove
 * \~chinese \brief 如果此属性值为 ture，则允许外界移动窗口的位置（如使用鼠标拖拽移动窗口），否则不允许。
  * \~chinese \note 此属性仅仅控制 dxcb 中的行为，不会影响窗口管理器的行为。
 */
bool DQuickWindowAttached::enableSystemMove() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemMove();
}

/*!
 * \~chinese \property DQuickWindowAttached::enableBlurWindow
 * \~chinese \brief 如果此属性为 true，则窗口有效区域内的背景将呈现出模糊效果，否则无特效。
 */
bool DQuickWindowAttached::enableBlurWindow() const
{
    D_DC(DQuickWindowAttached);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableBlurWindow();
}

/*!
 * \~chinese \brief DQuickWindowAttached::wmWindowTypes 返回此窗口在窗口管理器级别的窗口类型
 * \~chinese 需要注意的是，此值只是内部状态的记录，只会在调用 \a setWmWindowTypes
 * \~chinese 时更新，默认值为 \a DWindowManagerHelper::UnknowWindowType
 */
DWindowManagerHelper::WmWindowTypes DQuickWindowAttached::wmWindowTypes() const
{
    D_DC(DQuickWindowAttached);

    return d->wmWindowTypes;
}

/*!
 * \~chinese \brief DQuickWindowAttached::setEnabled　设置当前的窗口为 DTK 风格。
 * \~chinese \note 只能把默认风格设置为 DTK 风格，不能把 DTK 设置为默认风格。
 * \~chinese \param \a true 使用 DTK 风格， \a false 无效。
 */
void DQuickWindowAttached::setEnabled(bool e)
{
    D_D(DQuickWindowAttached);

    if (!e || e == d->explicitEnable)
        return;

    d->explicitEnable = e;
    d->updatePlatformHandle();

    if (isEnabled())
        Q_EMIT enabledChanged();
}

/*!
 * \~chinese \brief DQuickWindowAttached::setWindowRadius　设定窗口的圆角
 * \~chinese \param windowRadius　窗口的圆角值
 */
void DQuickWindowAttached::setWindowRadius(int windowRadius)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setWindowRadius(windowRadius);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setBorderWidth 设定边框的宽度
 * \~chinese \param borderWidth　边框的宽度
 */
void DQuickWindowAttached::setBorderWidth(int borderWidth)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setBorderWidth(borderWidth);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setBorderColor 设定边框的颜色
 * \~chinese \param borderColor　边框的颜色
 */
void DQuickWindowAttached::setBorderColor(const QColor &borderColor)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setBorderColor(borderColor);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setShadowRadius 设定阴影区域的圆角
 * \~chinese \param shadowRadius　阴影区域圆角大小
 */
void DQuickWindowAttached::setShadowRadius(int shadowRadius)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setShadowRadius(shadowRadius);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setShadowOffset 设定阴影区域的偏移距离
 * \~chinese \param shadowOffset　阴影区域的偏移距离
 */
void DQuickWindowAttached::setShadowOffset(const QPoint &shadowOffset)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setShadowOffset(shadowOffset);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setShadowColor 设定阴影的颜色
 * \~chinese \param shadowColor　阴影的颜色
 */
void DQuickWindowAttached::setShadowColor(const QColor &shadowColor)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setShadowColor(shadowColor);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setTranslucentBackground 设定时候擦除背景
 * \~chinese \param translucentBackground true擦除背景　false不擦除背景
 */
void DQuickWindowAttached::setTranslucentBackground(bool translucentBackground)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setTranslucentBackground(translucentBackground);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setEnableSystemResize　设定是否允许系统调整窗口大小
 * \~chinese \param enableSystemResize　true允许系统调整　false不允许系统调整
 */
void DQuickWindowAttached::setEnableSystemResize(bool enableSystemResize)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setEnableSystemResize(enableSystemResize);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setEnableSystemMove 设定时候允许系统移动窗口
 * \~chinese \param enableSystemMove　true允许移动　false不允许移动
 */
void DQuickWindowAttached::setEnableSystemMove(bool enableSystemMove)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setEnableSystemMove(enableSystemMove);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setEnableBlurWindow　设定窗口有效区域内的背景将呈现出模糊效果。
 * \~chinese \param enableBlurWindow true有特效　false无特效。
 */
void DQuickWindowAttached::setEnableBlurWindow(bool enableBlurWindow)
{
    D_D(DQuickWindowAttached);

    d->updatePlatformHandle();
    if (d->handle)
        d->handle->setEnableBlurWindow(enableBlurWindow);
}

/*!
 * \~chinese \brief DQuickWindowAttached::setWmWindowTypes 为此窗口设置与本地窗口管理器
 * \~chinese 息息相关的窗口类型，这些类型不保证在所有平台下都能生效，因此可能会影响程序
 * \~chinese 的跨平台行为，请尽量使用 \a QWindow::setFlags 设置所需要的窗口类型。
 * \~chinese \param wmWindowTypes 新的窗口类型，此枚举值可组合使用
 * \~chinese \note 调用此接口设置的窗口类型会与 \a QWindow::flags 中控制窗口类型的
 * \~chinese 部分共同生效
 */
void DQuickWindowAttached::setWmWindowTypes(DWindowManagerHelper::WmWindowTypes wmWindowTypes)
{
    D_D(DQuickWindowAttached);

    if (d->wmWindowTypes == wmWindowTypes)
        return;

    d->wmWindowTypes = wmWindowTypes;

    DWindowManagerHelper::setWmWindowTypes(window(), wmWindowTypes);
    Q_EMIT wmWindowTypesChanged(d->wmWindowTypes);
}

DQUICK_END_NAMESPACE
