load(dtk_lib)
include(private/private.pri)
include(scenegraph/scenegraph.pri)
TARGET = dtkdeclarative
TEMPLATE = lib
QT += dtkcore5.5 dtkgui5.5 core quick quick-private
CONFIG += internal_module c++11

# 龙芯架构上没有默认添加PT_GNU_STACK-section,所以此处手动指定一下
contains(QMAKE_HOST.arch, mips.*): QMAKE_LFLAGS_SHLIB += "-Wl,-z,noexecstack"

# for debian
isEmpty(LIB_INSTALL_DIR) {
    LIB_INSTALL_DIR = /usr/lib/$${QMAKE_HOST.arch}-linux-gnu
}

isEmpty(DTK_QML_APP_PLUGIN_PATH) {
    DTK_QML_APP_PLUGIN_PATH = $$LIB_INSTALL_DIR/$$TARGET/qml-app
}

DEFINES += DTK_QML_APP_PLUGIN_PATH=\\\"'$$DTK_QML_APP_PLUGIN_PATH'\\\"

include(src.pri)

includes.files += \
    $$PWD/*.h \
    $$PWD/DQuickProgressBar \
    $$PWD/DQuickWindow \
    $$PWD/DQuickView \
    $$PWD/DAppLoader \
    $$PWD/DQmlAppPluginInterface \
    $$PWD/DQMLGlobalObject \
    $$PWD/DPlatformThemeProxy \
    $$PWD/DQuickItemViewport \
    $$PWS/DQuickSystemPalette \
    $$PWS/DQuickProgressBar

includes.path = /usr/include/DDeclarative

pkgconfig.files += $$PWD/pkgconfig/dtkdeclarative.pc
pkgconfig.path = /usr/lib/$${QMAKE_HOST.arch}-linux-gnu/pkgconfig

unix {
    target.path = /usr/lib/$${QMAKE_HOST.arch}-linux-gnu
    INSTALLS += includes target pkgconfig
}


RESOURCES += $$PWD/dtkdeclarative_assets.qrc

DTK_MODULE_NAME=$$TARGET
load(dtk_build)

template.files += $$PWD/../examples/qml-app-template
template.path = /usr/share/qtcreator/templates/wizards/projects

INSTALLS += includes target template

CMAKE_CONTENT += "set(DTK_QML_APP_PLUGIN_PATH $${DTK_QML_APP_PLUGIN_PATH})"
MODULE_PRI_CONT += "QT.$${TARGET}.qml_apps = $${DTK_QML_APP_PLUGIN_PATH}"

load(dtk_cmake)
load(dtk_module)

DISTFILES += \
    shaders/quickitemduplicator.frag \
    shaders/quickitemduplicator-opaque.frag \
    DIcon.qml

RESOURCES += \
    dtkdeclarative_qml.qrc
