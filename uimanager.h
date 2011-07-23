#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QSystemTrayIcon>

#include "itoolmanager.h"
#include "tool.h"
#include "action.h"
#include "actionids.h"

class QMenuBar;
class QWidget;
class QToolBar;
class QStatusBar;
class QAction;
class MainWindow;
class QMenu;
class QSystemTrayIcon;
class ITaskList;
class TreeUi;
class ITreeUiProvider;
class ITaskControlManager;

class UiManager : public QObject, public IToolManager, public Tool
{
    Q_OBJECT
public:
    UiManager(QMenuBar *menuBar,
              QStatusBar *statusBar,
              QToolBar *toolBar,
              MainWindow *mainWindow);
    virtual ~UiManager();
public:
    void initManager();
public:
    virtual void onActionChanged(Actions::Actions id);
    virtual void addTool(ITool *tool);
public:
    virtual void init(IToolManager *manager);
    virtual bool isActionChecked(Actions::Actions action) const;
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    ITreeUiProvider* treeUi() const;
    void setTaskControlManager(ITaskControlManager *taskControlManager);
public slots:
    void onExit();
    void onShowToolbar();
    void onShowStatusbar();
    void onMinimize();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onMainWindowMinimized();
    void onMainWindowRestored();
    void onCurrentListChanged(ITaskList* newList);
    void onMaximizeTasklist();
private:
    void createMenu();
    void createTreeUi();
    void createTaskControls();
private:
    QMenuBar *m_menuBar;
    QList<Action> m_actions;
    QList<ITool*> m_tools;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QMap<Actions::Actions, ITool*> m_actionToolMap;
    QMap<Actions::Actions, QAction*> m_idActionMap;
    bool m_toolBarShown;
    bool m_statusBarShown;
    MainWindow *m_mainWindow;
    QMenu *m_trayMenu;
    QSystemTrayIcon *m_trayIcon;
    bool m_taskListMaximized;
    TreeUi *m_treeUi;
    ITaskControlManager *m_taskControlManager;
};

#endif // UIMANAGER_H
