#-------------------------------------------------
#
# Project created by QtCreator 2011-06-17T22:48:27
#
#-------------------------------------------------

QT       += core gui xml

TARGET = YourTodo
TEMPLATE = app

CONFIG  += qxt
QXT     += core gui

LIBS += user32.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    tasktree/treemodel.cpp \
    tasktree/task.cpp \
    tasktree/todolist.cpp \
    tasktree/xmltaskloader.cpp \
    tasktree/taskinfo.cpp \
    exceptions/exception.cpp \
    exceptions/loadtasksexception.cpp \
    exceptions/stopiterexception.cpp \
    tasktree/treeui.cpp \
    tasktree/treecolumndata.cpp \
    tasktree/tasktreeview.cpp \
    uimanager.cpp \
    tasktree/tasklist.cpp \
    action.cpp \
    filemanager.cpp \
    tool.cpp \
    application.cpp

HEADERS  += mainwindow.h \
    tasktree/treemodel.h \
    tasktree/task.h \
    tasktree/todolist.h \
    tasktree/itaskloader.h \
    tasktree/xmltaskloader.h \
    tasktree/taskinfo.h \
    exceptions/exception.h \
    exceptions/loadtasksexception.h \
    exceptions/stopiterexception.h \
    tasktree/treeui.h \
    tasktree/treecolumndata.h \
    tasktree/itreeuiprovider.h \
    tasktree/tasktreeview.h \
    uimanager.h \
    tasktree/tasklist.h \
    tasktree/itasklist.h \
    itoolmanager.h \
    itool.h \
    action.h \
    actionids.h \
    filemanager.h \
    tool.h \
    application.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
