#ifndef UIMANAGER_H
#define UIMANAGER_H

class QMenuBar;
class QWidget;
class QToolBar;
class QStatusBar;
class QAction;

#include <QObject>
#include <QList>
#include <QMap>
#include "itoolmanager.h"
#include "itool.h"
#include "action.h"
#include "actionids.h"

class UiManager : public QObject, public IToolManager, public ITool
{
    Q_OBJECT
public:
    UiManager(QWidget *parent, QMenuBar *menuBar, QStatusBar *statusBar, QToolBar *toolBar);
public:
    void Init();
public:
    virtual void onActionChanged(Actions::Actions id);
    virtual void addTool(ITool *tool);
public:
    virtual void getActions(QList<Actions::Actions> &actions) const;
    virtual bool isActionEnabled(Actions::Actions action) const;
    virtual bool isActionChecked(Actions::Actions action) const;
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
public slots:
    void onExit();
    void onNewTasklist();
    void onShowToolbar();
    void onShowStatusbar();
private:
    void CreateMenu();
private:
    QMenuBar *m_menuBar;
    QWidget *m_parentWindow;
    QList<Action> m_actions;
    QList<ITool*> m_tools;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QMap<Actions::Actions, ITool*> m_actionToolMap;
    QMap<Actions::Actions, QAction*> m_idActionMap;
};

#endif // UIMANAGER_H
