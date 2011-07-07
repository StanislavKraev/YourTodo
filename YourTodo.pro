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
    tasktree/taskstorage.cpp \
    tasktree/todolist.cpp \
    tasktree/xmltaskloader.cpp \
    tasktree/taskinfo.cpp \
    exceptions/exception.cpp \
    exceptions/loadtasksexception.cpp \
    exceptions/stopiterexception.cpp \
    tasktree/treeui.cpp \
    tasktree/treecolumndata.cpp \
    tasktree/tasktreeview.cpp \
    uimanager.cpp

HEADERS  += mainwindow.h \
    tasktree/treemodel.h \
    tasktree/task.h \
    tasktree/taskstorage.h \
    tasktree/todolist.h \
    tasktree/itaskstorage.h \
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
    uimanager.h

FORMS    += mainwindow.ui
