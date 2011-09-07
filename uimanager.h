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
#include "iuimanager.h"

class QMenuBar;
class QWidget;
class QToolBar;
class QStatusBar;
class QAction;
class MainWindow;
class QMenu;
class QSystemTrayIcon;
class QUndoStack;
class ITaskList;
class TreeUi;
class ITreeUiProvider;
class ITaskControlManager;
class IPreferences;
class IFileManager;

class UiManager : public QObject, public IToolManager, public Tool, public IUiManager
{
    Q_OBJECT
public:
    UiManager(QMenuBar *menuBar,
              QStatusBar *statusBar,
              QToolBar *toolBar,
              MainWindow *mainWindow,
              IPreferences *prefs,
              IFileManager *fileManager,
              QUndoStack *undoStack);
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
    virtual const QAction* action(Actions::Actions searchAction) const;
    virtual void setActionShortcut(Actions::Actions action, QKeySequence sequence);
public slots:
    void onExit();
    void onShowToolbar();
    void onShowStatusbar();
    void onMinimize();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void onMainWindowMinimized();
    void onMainWindowRestored();
    void onMainWindowClosing();
    void onCurrentListChanged(ITaskList* newList);
    void onMaximizeTasklist();
    void filenameChanged(QString title);
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
    IPreferences *m_prefs;
    IFileManager *m_fileManager;
    QUndoStack *m_undoStack;
};

#endif // UIMANAGER_H
