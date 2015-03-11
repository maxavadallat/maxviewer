
# Target
TARGET      = MaxViewer

# Template
TEMPLATE    = app

# Qt Modules/Config
QT          += core gui
QT          += quickwidgets
QT          += qml quick
QT          += widgets

# Icon
ICON        = resources/images/icons/maxviewer.icns

# Soures
SOURCES     += src/main.cpp \
            src/mainbrowserwindow.cpp \
            src/mainviewerwindow.cpp \
            src/browserdataobject.cpp \
            src/aboutform.cpp \
            src/preferencesdialog.cpp \
            src/myapp.cpp \
            src/mytreeview.cpp \
            src/myquickwidget.cpp \
            src/dirselectordialog.cpp \
            src/worker.cpp \
            src/renameseriesdialog.cpp \
            src/renamefiledialog.cpp \
            src/infodialog.cpp \
            src/comparedialog.cpp \
            src/imagecompareframe.cpp \
            src/imageframe.cpp \
            src/createdirdialog.cpp \
            src/confirmationdialog.cpp \
            src/utility.cpp \
            src/duplicatesearchresultdialog.cpp \
            src/duplicatelistmodel.cpp

# Headers
HEADERS     += src/mainbrowserwindow.h \
            src/mainviewerwindow.h \
            src/constants.h \
            src/browserdataobject.h \
            src/aboutform.h \
            src/preferencesdialog.h \
            src/myapp.h \
            src/mytreeview.h \
            src/myquickwidget.h \
            src/dirselectordialog.h \
            src/worker.h \
            src/renameseriesdialog.h \
            src/renamefiledialog.h \
            src/infodialog.h \
            src/comparedialog.h \
            src/imagecompareframe.h \
            src/imageframe.h \
            src/createdirdialog.h \
            src/confirmationdialog.h \
            src/utility.h \
            src/duplicatesearchresultdialog.h \
            src/duplicatelistmodel.h

# Forms
FORMS       += ui/mainbrowserwindow.ui \
            ui/mainviewerwindow.ui \
            ui/aboutform.ui \
            ui/preferencesdialog.ui \
            ui/dirselectordialog.ui \
            ui/renameseriesdialog.ui \
            ui/renamefiledialog.ui \
            ui/infodialog.ui \
            ui/comparedialog.ui \
            ui/createdirdialog.ui \
            ui/confirmationdialog.ui \
            ui/duplicatesearchresultdialog.ui

# Resources
RESOURCES   += maxviewer.qrc

# Other Files
OTHER_FILES += \
            createdmg.sh

# Libs
LIBS        += -framework Carbon

# Output/Intermediate Dirs
OBJECTS_DIR = ./objs
OBJMOC      = ./objs
MOC_DIR     = ./objs
UI_DIR      = ./objs
RCC_DIR     = ./objs


