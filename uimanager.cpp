#include <QMenuBar>
#include <QPair>
#include <QMessageBox>
#include <QToolBar>
#include <QStatusBar>
#include <QApplication>

#include "tasktree/treeui.h"
#include "tasktree/tasktreeview.h"
#include "itaskcontrolmanager.h"
#include "mainwindow.h"

#include "uimanager.h"

UiManager::UiManager(QMenuBar *menuBar, QStatusBar *statusBar,
                     QToolBar *toolBar, MainWindow *mainWindow,
                     IPreferences *prefs) :
    m_menuBar(menuBar),
    m_toolBar(toolBar),
    m_statusBar(statusBar),
    m_toolBarShown(true),
    m_statusBarShown(true),
    m_mainWindow(mainWindow),
    m_taskListMaximized(false),
    m_treeUi(0),
    m_taskControlManager(0),
    m_prefs(prefs)
{
    m_trayMenu = new QMenu(m_mainWindow);
    m_trayMenu->addAction("Exit", this, SLOT(onExit()));
    m_trayIcon = new QSystemTrayIcon(m_mainWindow);
    m_trayIcon->setContextMenu(m_trayMenu);
    m_trayIcon->setIcon(QIcon(":/icons/tray.gif"));
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
}

UiManager::~UiManager()
{
    if (m_trayIcon)
        m_trayIcon->deleteLater();
    if (m_trayMenu)
        m_trayMenu->deleteLater();
    if (m_treeUi)
        m_treeUi->deleteLater();
}

void UiManager::initManager()
{
    foreach(ITool *tool, m_tools)
        tool->init(this);

    createTreeUi();
    createTaskControls();
    createMenu();
}

void UiManager::createTreeUi()
{
    if (m_treeUi)
        delete m_treeUi;
    QFont treeViewFont = QFont(m_mainWindow->font());
    treeViewFont.setStrikeOut(true);
    m_treeUi = new TreeUi(treeViewFont, m_mainWindow->treeView(), m_prefs);
    m_treeUi->addColumn(TreeColumnData("O", nsTaskData::IconIndex, -1, TreeColumnData::ICONINDEX));
    m_treeUi->addColumn(TreeColumnData("!", nsTaskData::Priority, 22, TreeColumnData::PRIORITY));
    m_treeUi->addColumn(TreeColumnData("%", nsTaskData::PercentDone, 30));
    m_treeUi->addColumn(TreeColumnData("Pos", nsTaskData::Position, 24));
    m_treeUi->addColumn(TreeColumnData("Risk", nsTaskData::Risk, 40));
    m_treeUi->addColumn(TreeColumnData("Cost", nsTaskData::Cost, 40));
    m_treeUi->addColumn(TreeColumnData("Start date", nsTaskData::StartDate, 80));
    m_treeUi->addColumn(TreeColumnData("Done date", nsTaskData::DoneDate, 80));
    m_treeUi->addColumn(TreeColumnData("Creation date", nsTaskData::CreationDate, 80));
    m_treeUi->addColumn(TreeColumnData("Last mod", nsTaskData::LastModified, 100));
    m_treeUi->addColumn(TreeColumnData("CT", nsTaskData::CommentsType, 24));
    m_treeUi->addColumn(TreeColumnData("Comments", nsTaskData::Comments));
    m_treeUi->addColumn(TreeColumnData("Title", nsTaskData::Title, -1));
}

struct ActionHelper
{
    QString title;
    Actions::Actions id;
    QKeySequence keySequence;
    bool checkable;
    ActionHelper(QString title, Actions::Actions id, QKeySequence sequence = QKeySequence())
    {
        this->title = title;
        this->id = id;
        this->keySequence = sequence;
        this->checkable = false;
    }
    ActionHelper(QString title, Actions::Actions id, bool checkable, QKeySequence sequence = QKeySequence())
    {
        this->title = title;
        this->id = id;
        this->keySequence = sequence;
        this->checkable = checkable;
    }
};

struct Separator : ActionHelper
{
    Separator() : ActionHelper(QString::null, Actions::Undefined){}
};

void UiManager::createMenu()
{
    m_menuBar->clear();

    foreach(ITool *tool, m_tools)
    {
        QList<Actions::Actions> actions;
        tool->getActions(actions);
        foreach(Actions::Actions id, actions)
            m_actionToolMap[id] = tool;
    }


    QMenu *fileMenu = m_menuBar->addMenu("&File");
    QMenu *newTaskMenu = m_menuBar->addMenu("&New Task");
    QMenu *editMenu = m_menuBar->addMenu("&Edit");
    QMenu *viewMenu = m_menuBar->addMenu("&View");
    QMenu *moveMenu = m_menuBar->addMenu("&Move");

    QList<ActionHelper> fileMenuActions;
    fileMenuActions
            << ActionHelper("New Tasklist", Actions::FileNew, QKeySequence(Qt::ALT + Qt::Key_N))
            << Separator()
            << ActionHelper("Open Tasklist", Actions::FileOpen)
            << Separator()
            << ActionHelper("Save Tasklist", Actions::FileSave)
            << ActionHelper("Save Tasklist as...", Actions::FileSaveAs)
            //<< ActionHelper("Save All", Actions::FileSaveAll)
            //<< Separator()
            //<< ActionHelper("Close Tasklist", Actions::FileClose)
            //<< ActionHelper("Close All", Actions::FileCloseAll)
            << Separator()
            << ActionHelper("Minimize to System Tray", Actions::FileMinimize)
            << Separator()
            << ActionHelper("Exit", Actions::FileExit, QKeySequence(Qt::ALT + Qt::Key_F4));

    QList<ActionHelper> newTaskMenuActions;
    newTaskMenuActions
            << ActionHelper("New Task Above", Actions::NewTaskAbove)
            << ActionHelper("New Task Below", Actions::NewTaskBelow)
            << ActionHelper("New Subtask", Actions::NewSubtask);

    QList<ActionHelper> editMenuActions;
    editMenuActions
            << ActionHelper("Undo", Actions::EditUndo)
            << ActionHelper("Redo", Actions::EditRedo)
            << Separator()
            << ActionHelper("Cut", Actions::EditCut)
            << ActionHelper("Copy", Actions::EditCopy)
            << ActionHelper("Paste", Actions::EditPaste)
            << Separator()
            << ActionHelper("Delete Selected", Actions::EditDeleteSelected)
            << ActionHelper("Delete All", Actions::EditDeleteAll)
            << Separator()
            << ActionHelper("Select All", Actions::EditSelectAll, QKeySequence(Qt::CTRL + Qt::Key_A))
            << Separator()
            << ActionHelper("Preferences...", Actions::EditPreferences);

    QList<ActionHelper> viewMenuActions;
    viewMenuActions
            << ActionHelper("Maximize Tasklist", Actions::ViewMaxTasklist, true, QKeySequence(Qt::CTRL + Qt::Key_M))
            << ActionHelper("Maximize Comments", Actions::ViewMaxComments)
            << Separator()
            << ActionHelper("Expand All", Actions::ViewExpandAll)
            << ActionHelper("Collapse All", Actions::ViewCollapseAll)
            << Separator()
            << ActionHelper("Select Columns...", Actions::ViewSelectColumns)
            << Separator()
            << ActionHelper("Show Toolbar", Actions::ViewShowToolbar, true)
            << ActionHelper("Show Statusbar", Actions::ViewShowStatusbar, true);

    QList<ActionHelper> moveMenuActions;
    moveMenuActions
            << ActionHelper("Move up", Actions::MoveUp)
            << ActionHelper("Move down", Actions::MoveDown)
            << ActionHelper("Move left", Actions::MoveLeft)
            << ActionHelper("Move right", Actions::MoveRight);

    typedef QPair<QMenu*, QList<ActionHelper> > MenuActionsPair;
    QList<MenuActionsPair> menuActions;
    menuActions.append(MenuActionsPair(fileMenu, fileMenuActions));
    menuActions.append(MenuActionsPair(newTaskMenu, newTaskMenuActions));
    menuActions.append(MenuActionsPair(editMenu, editMenuActions));
    menuActions.append(MenuActionsPair(viewMenu, viewMenuActions));
    menuActions.append(MenuActionsPair(moveMenu, moveMenuActions));

    foreach(MenuActionsPair pair, menuActions)
    {
        QMenu *menu = pair.first;
        QList<ActionHelper> actionList = pair.second;
        foreach(ActionHelper helper, actionList)
        {
            if (helper.id == Actions::Undefined)
            {
                menu->addSeparator();
                continue;
            }

            ITool *tool = 0;
            if (m_actionToolMap.contains(helper.id))
                tool = m_actionToolMap[helper.id];
            bool enabled = (tool != 0) && (tool->isActionEnabled(helper.id));
            bool checked = tool ? tool->isActionChecked(helper.id) : false;
            const char *slot = tool ? tool->getActionSlot(helper.id) : 0;
            QObject *obj = tool ? tool->getReciever() : 0;
            QAction *a = (obj && slot) ?
                menu->addAction(helper.title, obj, slot) :
                menu->addAction(helper.title);
            m_idActionMap[helper.id] = a;
            if (!helper.keySequence.isEmpty())
                a->setShortcut(helper.keySequence);
            a->setParent(this);
            a->setEnabled(enabled);
            a->setCheckable(helper.checkable);
            if (helper.checkable)
                a->setChecked(checked);
        }
    }
}

void UiManager::onExit()
{
    qApp->quit();
}

void UiManager::onActionChanged(Actions::Actions id)
{
    if (m_actionToolMap.contains(id))
    {
        ITool *tool = m_actionToolMap[id];
        bool enabled = tool->isActionEnabled(id);
        bool checked = tool->isActionChecked(id);
        QAction *action = m_idActionMap[id];
        if (action->isCheckable())
            action->setChecked(checked);
        action->setEnabled(enabled);
    }
}

void UiManager::addTool(ITool *tool)
{
    if (!tool || m_tools.contains(tool))
        return;
    m_tools.append(tool);
}

const char * UiManager::getActionSlot(Actions::Actions action) const
{
    switch (action)
    {
    case Actions::FileExit:
        return SLOT(onExit());
    case Actions::ViewShowStatusbar:
        return SLOT(onShowStatusbar());
    case Actions::ViewShowToolbar:
        return SLOT(onShowToolbar());
    case Actions::FileMinimize:
        return SLOT(onMinimize());
    case Actions::ViewMaxTasklist:
        return SLOT(onMaximizeTasklist());
    default:
        break;
    }
    return 0;
}

QObject* UiManager::getReciever()
{
    return this;
}

void UiManager::onShowToolbar()
{
    if (m_toolBar->isVisible())
    {
        m_toolBar->hide();
        m_toolBarShown = false;
    }
    else
    {
        m_toolBar->show();
        m_toolBarShown = true;
    }
    onActionChanged(Actions::ViewShowToolbar);
}

void UiManager::onShowStatusbar()
{
    if (m_statusBar->isVisible())
    {
        m_statusBar->hide();
        m_statusBarShown = false;
    }
    else
    {
        m_statusBar->show();
        m_statusBarShown = true;
    }
    onActionChanged(Actions::ViewShowStatusbar);
}

bool UiManager::isActionChecked(Actions::Actions action) const
{
    switch (action)
    {
    case Actions::ViewShowStatusbar:
        return m_statusBarShown;
    case Actions::ViewShowToolbar:
        return m_toolBarShown;
    case Actions::ViewMaxTasklist:
        return m_taskListMaximized;
    default:
        break;
    }

    return Tool::isActionChecked(action);
}

void UiManager::onMinimize()
{
    m_mainWindow->hide();
    m_trayIcon->show();
}

void UiManager::init(IToolManager *manager)
{
    (void)manager;
    addAction(Actions::FileExit);
    addAction(Actions::ViewShowStatusbar);
    addAction(Actions::ViewShowToolbar);
    addAction(Actions::FileMinimize);
    addAction(Actions::ViewMaxTasklist);
}

void UiManager::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
 {
     switch (reason)
     {
     case QSystemTrayIcon::Trigger:
     case QSystemTrayIcon::DoubleClick:
         m_trayIcon->hide();
         m_mainWindow->showNormal();
         m_mainWindow->activateWindow();
         break;
     default:
         break;
     }
}

void UiManager::onMainWindowMinimized()
{
    m_trayIcon->show();
}

void UiManager::onMainWindowRestored()
{
    m_trayIcon->hide();
}

void UiManager::onCurrentListChanged(ITaskList* newList)
{
    m_mainWindow->updateTreeModel(newList, m_treeUi);
    m_treeUi->init();
}

void UiManager::onMaximizeTasklist()
{
    m_taskListMaximized = !m_taskListMaximized;
    m_mainWindow->maximizeTaskList(m_taskListMaximized);
    onActionChanged(Actions::ViewMaxTasklist);
}

void UiManager::createTaskControls()
{
    m_taskControlManager->createTaskControls(m_mainWindow->commentsControl());
}

ITreeUiProvider * UiManager::treeUi() const
{
    return m_treeUi;
}

void UiManager::setTaskControlManager(ITaskControlManager *taskControlManager)
{
    m_taskControlManager = taskControlManager;
}
