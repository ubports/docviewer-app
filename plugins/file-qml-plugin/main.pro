TEMPLATE = lib
TARGET = filePlugin
QT += qml quick
CONFIG += qt plugin

#comment in the following line to disable traces
DEFINES += QT_NO_DEBUG_OUTPUT

TARGET = $$qtLibraryTarget($$TARGET)
uri = org.docviewer.file

INCLUDEPATH += .

# Input
HEADERS += backend.h \
    docviewerFile.h \
    fileReader.h
SOURCES += backend.cpp \
    docviewerFile.cpp \
    fileReader.cpp

# Install path for the plugin
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
target.path = $$installPath
INSTALLS += target

# find files
QMLDIR_FILE = qmldir

# make visible to qt creator
OTHER_FILES += $$QMLDIR_FILE 

# create install targets for files
qmldir.path = $$installPath
qmldir.files = $$QMLDIR_FILE

INSTALLS += qmldir

