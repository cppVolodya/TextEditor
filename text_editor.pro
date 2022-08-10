# --------------------------------------------------------------------------- #
# basic settings
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) : QT += widgets

CONFIG += c++17
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# source files
SOURCES += \
    sources/main.cpp \
    sources/text_editor.cpp \
    sources/status_bar.cpp
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# header files
HEADERS += \
    headers/text_editor.hpp \
    headers/status_bar.hpp
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# form files
FORMS += \
    forms_ui/text_editor.ui
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# resource files
RESOURCES += \
    resources.qrc
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# include paths
INCLUDEPATH += \
    headers
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# other files
RC_FILE += \
    config/text_editor.rc

OTHER_FILES += \
    config/text_editor.rc
# --------------------------------------------------------------------------- #


# --------------------------------------------------------------------------- #
# default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
# --------------------------------------------------------------------------- #