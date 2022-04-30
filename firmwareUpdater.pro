include($$PWD/firmwareUpdater.pri)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++2a

# Properties definition
FU_VERSIONMAJ = 2
FU_VERSIONMIN = 0
FU_VERSIONSUB = 0
FU_VERSIONREV = 0
FU_PRODUCTNAME = "Firmware Updater"
FU_PRODUCTVERSION = $${FU_VERSIONMAJ}"."$${FU_VERSIONMIN}"."$${FU_VERSIONSUB}"."$${FU_VERSIONREV}
FU_FILEDESCRIPTION = "$${FU_PRODUCTNAME}"
FU_FILEVERSION = "$${FU_PRODUCTVERSION}"
FU_COMPANYNAME = "HOME"
FU_COMMENTS = "Firmware Updater application"
FU_LEGALTRADEMARKS = "Firmware Updater is a trademark of HOME"
FU_LEGALCOPYRIGHT = "Copyright (c) HOME, Jerry Belaston. All rights reserved."
FU_BUNDLEIDENTIFIER = "com.home.firmwareUpdater"
FU_URLABOUTINFO = "http://www.github.com/firmwareUpdater"
macx: FU_ICONFILENAME = Icon.icns

# Substitutes
QMAKE_SUBSTITUTES += config.h.in
macx: QMAKE_SUBSTITUTES += res/macos/qmake/Info.plist.in

# Resources definition
ICON = res/logo.png
macx: ICON = res/macos/$${FU_ICONFILENAME}
macx: QMAKE_INFO_PLIST = $${OUT_PWD}/res/macos/qmake/Info.plist
win32:RC_FILE += res/win/qmake/resources.rc
RESOURCES += res/main.qrc

# Compile flags definition
DEFINES += QMAKE_FLAGS_USED
msvc {
    QMAKE_CXXFLAGS += /W4
    QMAKE_CXXFLAGS += /WX
}
clang {
    QMAKE_CXXFLAGS += -Wall
    QMAKE_CXXFLAGS += -Werror
    QMAKE_CXXFLAGS += -g
}
macx {
    QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/core/controller/controller.cpp \
    src/core/controller/controllerMulti.cpp \
    src/core/controller/controllerSingle.cpp \
    src/core/model/device/private/deviceSerialization.cpp \
    src/core/model/device/discovery.cpp \
    src/core/model/device/uploadManager.cpp \
    src/core/model/private/taskManager.cpp \
    src/core/model/templateInfo/private/templateSerialization.cpp \
    src/core/model/templateInfo/server.cpp \
    src/core/model/model.cpp \
    src/core/toolkit/timerTask.cpp \
    src/core/core.cpp \
    src/ui/toolkit/dataDownloader.cpp \
    src/ui/toolkit/qssHelper.cpp \
    src/ui/toolkit/stepperWidget.cpp \
    src/ui/view/mainWindow/mainWindowView.cpp \
    src/ui/view/workspace/common/templateInfo/templateInfo.cpp \
    src/ui/view/workspace/common/templateInfo/templateStep.cpp \
    src/ui/view/workspace/common/imageCacheManager.cpp \
    src/ui/view/workspace/common/modeZone.cpp \
    src/ui/view/workspace/modeMulti/projectZone/deviceWidget.cpp \
    src/ui/view/workspace/modeMulti/projectZone/projectZoneMulti.cpp \
    src/ui/view/workspace/modeMulti/multiModeView.cpp \
    src/ui/view/workspace/modeSingle/commander/commander.cpp \
    src/ui/view/workspace/modeSingle/projectZone/projectZoneSingle.cpp \
    src/ui/view/workspace/modeSingle/modeSingleView.cpp \
    src/ui/view/workspace/workspace.cpp \
    src/main.cpp

HEADERS += \
    src/core/common/type.hpp \
    src/core/controller/controller.hpp \
    src/core/controller/controllerMulti.hpp \
    src/core/controller/controllerSingle.hpp \
    src/core/controller/viewInterface.hpp \
    src/core/model/device/private/defaultDevices.hpp \
    src/core/model/device/private/deviceSerialization.hpp \
    src/core/model/device/discovery.hpp \
    src/core/model/device/uploadManager.hpp \
    src/core/model/private/taskManager.hpp \
    src/core/model/templateInfo/private/defaultTemplates.hpp \
    src/core/model/templateInfo/private/templateSerialization.hpp \
    src/core/model/templateInfo/server.hpp \
    src/core/model/model.hpp \
    src/core/toolkit/timerTask.hpp \
    src/core/core.hpp \
    src/ui/toolkit/dataDownloader.hpp \
    src/ui/toolkit/qssHelper.hpp \
    src/ui/toolkit/stepperWidget.hpp \
    src/ui/view/mainWindow/mainWindowView.hpp \
    src/ui/view/workspace/common/templateInfo/templateInfo.hpp \
    src/ui/view/workspace/common/templateInfo/templateStep.hpp \
    src/ui/view/workspace/common/imageCacheManager.hpp \
    src/ui/view/workspace/common/modeZone.hpp \
    src/ui/view/workspace/modeMulti/projectZone/deviceWidget.hpp \
    src/ui/view/workspace/modeMulti/projectZone/projectZoneMulti.hpp \
    src/ui/view/workspace/modeMulti/multiModeView.hpp \
    src/ui/view/workspace/modeSingle/commander/commander.hpp \
    src/ui/view/workspace/modeSingle/projectZone/projectZoneSingle.hpp \
    src/ui/view/workspace/modeSingle/modeSingleView.hpp \
    src/ui/view/workspace/workspace.hpp

# Copy main.qss to the destination dir
win32 {
    CONFIG(debug, debug|release) {
        TARGETDIR = $${OUT_PWD}/debug
    }
    else {
        TARGETDIR = $${OUT_PWD}/release
    }
}
macx {
    CONFIG(debug, debug|release) {
        TARGETDIR = $${OUT_PWD}/$${TARGET}.app/Contents/MacOS
    }
    else {
        TARGETDIR = $${OUT_PWD}/$${TARGET}.app/Contents/MacOS
    }
}
copyFileLater($$PWD/res/main.qss, $${TARGETDIR})

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
