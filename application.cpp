#include "mainwindow.h"

#include "application.h"

Application::Application(int &argc, char **argv) : QApplication(argc, argv)
{
    m_mainWindow = new MainWindow();
    m_mainWindow->SetupEventFilter();
    m_mainWindow->show();
}

Application::~Application()
{
    delete m_mainWindow;
}

MainWindow * Application::mainWindow()
{
    return m_mainWindow;
}
