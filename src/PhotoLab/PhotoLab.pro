QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Model/SimpleFilter.cpp \
    ../Model/Sliders.cpp \
    Filters/bright_contrast.cpp \
    Filters/hsl_filters.cpp \
    Filters/hsv_filters.cpp \
    mygraphicsview.cpp \
    main.cpp \
    mainwindow.cpp \
    ../Controller/controller.cpp \
    ../Model/Photo.cpp \
    ../Model/Filter.cpp \
    Kernel/user_kernel.cpp

HEADERS += \
    ../Model/SimpleFilter.h \
    ../Model/Sliders.h \
    Filters/bright_contrast.h \
    Filters/hsl_filters.h \
    Filters/hsv_filters.h \
    mygraphicsview.h \
    mainwindow.h \
    ../Controller/controller.h \
    ../Model/Photo.h \
    ../Model/Matrix.h \
    ../Model/Filter.h \
    Kernel/user_kernel.h

FORMS += \
    Filters/bright_contrast.ui \
    Filters/hsl_filters.ui \
    Filters/hsv_filters.ui \
    mainwindow.ui \
    Kernel/user_kernel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DEFINES += HOME_DIR=\\\"$(HOME)\\\"

RESOURCES += \
    resource.qrc

ICON = logo.icns
