#-------------------------------------------------
#
# Project created by QtCreator 2011-06-17T22:48:27
#
#-------------------------------------------------

QT       += core gui xml

TARGET = YourTodo
TEMPLATE = app


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
    action.cpp

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
    actionids.h

FORMS    += mainwindow.ui
