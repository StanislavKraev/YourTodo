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
class QMainWindow;
class QMenu;
class QSystemTrayIcon;

class UiManager : public QObject, public IToolManager, public Tool
{
    Q_OBJECT
public:
    UiManager(QWidget *parent,
              QMenuBar *menuBar,
              QStatusBar *statusBar,
              QToolBar *toolBar,
              QMainWindow *mainWindow);
public:
    void initManager();
    void onMainWindowMinimized();
    void onMainWindowRestored();
public:
    virtual void onActionChanged(Actions::Actions id);
    virtual void addTool(ITool *tool);
public:
    virtual void init(IToolManager *manager);
    virtual bool isActionChecked(Actions::Actions action) const;
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
public slots:
    void onExit();
    void onShowToolbar();
    void onShowStatusbar();
    void onMinimize();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
private:
    void createMenu();
private:
    QMenuBar *m_menuBar;
    QWidget *m_parentWindow;
    QList<Action> m_actions;
    QList<ITool*> m_tools;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QMap<Actions::Actions, ITool*> m_actionToolMap;
    QMap<Actions::Actions, QAction*> m_idActionMap;
    bool m_toolBarShown;
    bool m_statusBarShown;
    QMainWindow *m_mainWindow;
    QMenu *m_trayMenu;
    QSystemTrayIcon *m_trayIcon;
};

#endif // UIMANAGER_H
