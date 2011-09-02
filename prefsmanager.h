#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include <QWidget>
#include <QSet>

#include "tool.h"
#include "ipreferences.h"

class IToolManager;
class PreferencesModel;

class PrefsManager : public QObject, public Tool, public IPreferences
{
    Q_OBJECT
public:
    PrefsManager(QWidget *parent);
    virtual ~PrefsManager();
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    virtual bool isActionEnabled(Actions::Actions action) const;
public:
    virtual bool isColumnSelected(nsTaskData::TaskDataMember member) const;
    virtual bool saveOnMinimize() const;
    virtual bool saveOnExit() const;
public slots:
    void showPreferences();
private:
    QSet<nsTaskData::TaskDataMember> m_selectedColumns;
    IToolManager *m_manager;
    QWidget *m_parentWindow;
    PreferencesModel *m_prefs;
};

#endif // PREFSMANAGER_H
