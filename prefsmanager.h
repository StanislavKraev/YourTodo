#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include <QWidget>
#include <QSet>

#include "tool.h"
#include "ipreferences.h"

class IToolManager;
class PreferencesModel;
class IUiManager;

class PrefsManager : public QObject, public Tool, public IPreferences
{
    Q_OBJECT
public:
    PrefsManager(QWidget *parent);
    virtual ~PrefsManager();
public:
    void setUiManager(IUiManager *uiManager);
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    virtual bool isActionEnabled(Actions::Actions action) const;
public:
    virtual bool isColumnSelected(nsTaskData::TaskDataMember member) const;
    virtual bool saveOnMinimize() const;
    virtual bool saveOnExit() const;
    virtual QKeySequence shortcutForAction(Actions::Actions id) const;
public slots:
    void showPreferences();
private slots:
    void shortcutChanged(Actions::Actions action, QKeySequence sequence);
private:
    QSet<nsTaskData::TaskDataMember> m_selectedColumns;
    IToolManager *m_manager;
    QWidget *m_parentWindow;
    PreferencesModel *m_prefs;
    IUiManager *m_uiManager;
};

#endif // PREFSMANAGER_H
