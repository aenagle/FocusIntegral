QT += widgets

CONFIG += c++17

RC_ICONS = myicon.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainwindow_helpers.cpp \
    mainwindow_timer.cpp \
    mainwindow_tasks.cpp \
    mainwindow_integrals.cpp \
    mainwindow_helpers.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    CMakeLists \
    README.md
