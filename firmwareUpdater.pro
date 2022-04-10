QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++2a

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += src

SOURCES += \
    src/core/controller/controller.cpp \
    src/core/model/private/templatesSerialization.cpp \
    src/core/model/server.cpp \
    src/core/toolkit/timerTask.cpp \
    src/main.cpp \
    src/ui/toolkit/dataDownloader.cpp \
    src/ui/toolkit/stepperWidget.cpp \
    src/ui/view/main/mainView.cpp \
    src/ui/view/templateBrowser/templateBrowserView.cpp \
    src/ui/view/templateInfo/templateInfoView.cpp \
    src/ui/view/templateInfo/templateStepView.cpp \
    src/ui/view/welcome/welcomeView.cpp

HEADERS += \
    src/core/common/type.hpp \
    src/core/controller/controller.hpp \
    src/core/controller/viewInterface.hpp \
    src/core/model/private/defaultTemplates.hpp \
    src/core/model/private/templatesSerialization.hpp \
    src/core/model/server.hpp \
    src/core/toolkit/timerTask.hpp \
    src/ui/toolkit/dataDownloader.hpp \
    src/ui/toolkit/stepperWidget.hpp \
    src/ui/view/main/mainView.hpp \
    src/ui/view/templateBrowser/templateBrowserView.hpp \
    src/ui/view/templateInfo/templateInfoView.hpp \
    src/ui/view/templateInfo/templateStepView.hpp \
    src/ui/view/welcome/welcomeView.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
