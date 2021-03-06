#include <QCoreApplication>
#include <QSettings>
#include <QUndoStack>

#include "mainwindow.h"
#include "filemanager.h"
#include "selectiontool.h"
#include "taskeditortool.h"
#include "uimanager.h"
#include "taskcontrolmanager.h"
#include "prefsmanager.h"
#include "tasktree/itasklist.h"

#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    createSettings();
    m_undoStack = new QUndoStack(this);

    m_mainWindow = new MainWindow();
    m_prefsManager = new PrefsManager(m_mainWindow);
    m_fileManager = new FileManager(m_mainWindow);
    m_selectionTool = new SelectionTool(m_mainWindow->selectionModel(), m_mainWindow->model(), m_mainWindow->treeView());
    m_taskEditTool = new TaskEditorTool(m_mainWindow->treeView(), m_undoStack);

    m_uiManager = new UiManager(m_mainWindow->menuBar(),
                                m_mainWindow->statusBar(), m_mainWindow->toolBar(),
                                m_mainWindow, m_prefsManager, m_fileManager, m_undoStack);
    m_taskControlManager = new TaskControlManager(m_mainWindow->controlsArea(),
                                                  m_prefsManager);
    m_uiManager->setTaskControlManager(m_taskControlManager);
    m_uiManager->addTool(m_uiManager);
    m_uiManager->addTool(m_fileManager);
    m_uiManager->addTool(m_selectionTool);
    m_uiManager->addTool(m_taskEditTool);
    m_uiManager->addTool(m_prefsManager);
    m_uiManager->initManager();
    m_prefsManager->setUiManager(m_uiManager);
    m_mainWindow->setUiManager(m_uiManager);
    m_mainWindow->SetupEventFilter();

    connect(m_mainWindow, SIGNAL(onMainWindowMinimized()), m_uiManager, SLOT(onMainWindowMinimized()));
    connect(m_mainWindow, SIGNAL(onMainWindowRestored()), m_uiManager, SLOT(onMainWindowRestored()));
    connect(m_mainWindow, SIGNAL(onMainWindowClosing()), m_uiManager, SLOT(onMainWindowClosing()));
    connect(m_fileManager, SIGNAL(currentListChanged(ITaskList*)), m_uiManager, SLOT(onCurrentListChanged(ITaskList*)));
    connect(m_fileManager, SIGNAL(currentListChanged(ITaskList*)),
            m_taskControlManager, SLOT(onCurrentListChanged(ITaskList*)));
    connect(m_fileManager, SIGNAL(filenameChanged(QString)), m_uiManager, SLOT(filenameChanged(QString)));
    connect(m_mainWindow, SIGNAL(onModelsChanged(QItemSelectionModel*,QAbstractItemModel*)),
            m_selectionTool, SLOT(onModelsChanged(QItemSelectionModel*,QAbstractItemModel*)));
    connect(m_selectionTool, SIGNAL(selectionChanged(QItemSelectionModel*)),
            m_taskControlManager, SLOT(selectionChanged(QItemSelectionModel*)));
    connect(m_selectionTool, SIGNAL(selectionChanged(QItemSelectionModel*)),
            m_taskEditTool, SLOT(selectionChanged(QItemSelectionModel*)));
    connect(m_prefsManager, SIGNAL(onTopChanged(bool)),
            m_mainWindow, SLOT(onTopChanged(bool)));

    connect(m_selectionTool, SIGNAL(currentChanged(Task::Ptr)),
            m_uiManager, SLOT(currentChanged(Task::Ptr)));

    m_fileManager->startUp();
    m_mainWindow->show();
}

Application::~Application()
{
    m_uiManager->deleteLater();
    delete m_taskEditTool;
    delete m_selectionTool;
    delete m_fileManager;
    delete m_prefsManager;
    m_mainWindow->deleteLater();
    m_undoStack->deleteLater();
}

MainWindow * Application::mainWindow()
{
    return m_mainWindow;
}

void Application::createSettings()
{
    QCoreApplication::setOrganizationName("OpenSource");
    QCoreApplication::setOrganizationDomain(".org");
    QCoreApplication::setApplicationName("YourTodo");
}
