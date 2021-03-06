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

win32 {
LIBS += user32.lib
}

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
    application.cpp \
    tasktree/itasksaver.cpp \
    tasktree/xmltasksaver.cpp \
    utils.cpp \
    tasktree/delegates.cpp \
    selectiontool.cpp \
    widgets/titledwidget.cpp \
    widgets/flowlayout.cpp \
    widgets/prioritywidget.cpp \
    widgets/spinnerwidget.cpp \
    taskcontrolmanager.cpp \
    prefsmanager.cpp \
    widgets/datawidget.cpp \
    widgets/datawidgetmodel.cpp \
    widgets/texteditmodel.cpp \
    widgets/floatpointingwidget.cpp \
    widgets/comboboxwidget.cpp \
    taskeditortool.cpp \
    prefsdialog.cpp \
    preferencesmodel.cpp \
    widgets/keyassignwidget.cpp \
    undoredo/deleteselectedtaskscmd.cpp

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
    application.h \
    tasktree/itasksaver.h \
    tasktree/xmltasksaver.h \
    utils.h \
    tasktree/delegates.h \
    selectiontool.h \
    widgets/titledwidget.h \
    widgets/flowlayout.h \
    widgets/prioritywidget.h \
    widgets/spinnerwidget.h \
    taskcontrolmanager.h \
    itaskcontrolmanager.h \
    prefsmanager.h \
    ipreferences.h \
    widgets/datawidget.h \
    widgets/datawidgetmodel.h \
    tasktree/itaskwatcher.h \
    widgets/texteditmodel.h \
    widgets/floatpointingwidget.h \
    widgets/comboboxwidget.h \
    taskeditortool.h \
    prefsdialog.h \
    preferencesmodel.h \
    ifilemanager.h \
    iuimanager.h \
    widgets/keyassignwidget.h \
    undoredo/deleteselectedtaskscmd.h

FORMS    += mainwindow.ui \
    prefsdialog.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    add_column_checklist.txt
