#include <QMenuBar>
#include <QApplication>

#include "uimanager.h"

UiManager::UiManager(QMenuBar *menuBar) :
    m_menuBar(menuBar)
{
}

void UiManager::Init()
{
    CreateMenu();
}

void UiManager::CreateMenu()
{
    m_menuBar->clear();

    // File menu
    QMenu *subMenu = m_menuBar->addMenu("File");
    subMenu->addAction("New Tasklist");
    subMenu->addSeparator();
    subMenu->addAction("Open Tasklist");
    subMenu->addSeparator();
    subMenu->addAction("Save Tasklist");
    subMenu->addAction("Save Tasklist as...");
    subMenu->addAction("Save All");
    subMenu->addSeparator();
    subMenu->addAction("Close Tasklist");
    subMenu->addAction("Close All");
    subMenu->addSeparator();
    subMenu->addAction("Minimize to System Tray");
    subMenu->addAction("Exit", this, SLOT(onExit()));

    // New Task menu
    subMenu = m_menuBar->addMenu("New Task");
    subMenu->addAction("New Task Above");
    subMenu->addAction("New Task Below");
    subMenu->addAction("New Subtask");

    // Edit menu
    subMenu = m_menuBar->addMenu("Edit");
    subMenu->addAction("Undo");
    subMenu->addAction("Redo");
    subMenu->addSeparator();
    subMenu->addAction("Cut");
    subMenu->addAction("Copy");
    subMenu->addAction("Paste");
    subMenu->addSeparator();
    subMenu->addAction("Delete Selected");
    subMenu->addAction("Delete All");
    subMenu->addSeparator();
    subMenu->addAction("Select All");
    subMenu->addSeparator();
    subMenu->addAction("Preferences...");

    // View menu
    subMenu = m_menuBar->addMenu("View");
    subMenu->addAction("Maximize Tasklist");
    subMenu->addAction("Maximize Comments");
    subMenu->addSeparator();
    subMenu->addAction("Expand All");
    subMenu->addAction("Collapse All");
    subMenu->addSeparator();
    subMenu->addAction("Select Columns...");
    subMenu->addSeparator();
    subMenu->addAction("Show Toolbar");
    subMenu->addAction("Show Statusbar");

    // Move menu
    subMenu = m_menuBar->addMenu("Move");
    subMenu->addAction("Move up");
    subMenu->addAction("Move down");
    subMenu->addAction("Move left");
    subMenu->addAction("Move right");
}

void UiManager::onExit()
{
    qApp->quit();
}
