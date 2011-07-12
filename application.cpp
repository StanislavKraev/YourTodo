#include "mainwindow.h"
#include "filemanager.h"
#include "uimanager.h"

#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{

    m_mainWindow = new MainWindow();
    m_mainWindow->SetupEventFilter();
    m_fileManager = new FileManager(m_mainWindow);

    m_uiManager = new UiManager(m_mainWindow->menuBar(),
                                m_mainWindow->statusBar(), m_mainWindow->toolBar(), m_mainWindow);
    m_uiManager->addTool(m_uiManager);
    m_uiManager->addTool(m_fileManager);
    m_uiManager->initManager();

    connect(m_mainWindow, SIGNAL(onMainWindowMinimized()), m_uiManager, SLOT(onMainWindowMinimized()));
    connect(m_mainWindow, SIGNAL(onMainWindowRestored()), m_uiManager, SLOT(onMainWindowRestored()));
    connect(m_fileManager, SIGNAL(currentListChanged(ITaskList*)), m_uiManager, SLOT(onCurrentListChanged(ITaskList*)));
    m_mainWindow->show();
}

Application::~Application()
{
    m_uiManager->deleteLater();
    m_mainWindow->deleteLater();
    delete m_fileManager;
}

MainWindow * Application::mainWindow()
{
    return m_mainWindow;
}
