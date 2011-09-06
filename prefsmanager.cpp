#include <QMessageBox>

#include "prefsdialog.h"
#include "preferencesmodel.h"
#include "iuimanager.h"
#include "prefsmanager.h"

PrefsManager::PrefsManager(QWidget *parent) :
    m_manager(0), m_parentWindow(parent), m_uiManager(0)
{
    m_prefs = new PreferencesModel();
    m_selectedColumns << nsTaskData::Title
                      << nsTaskData::Priority
                      << nsTaskData::IconIndex
                      << nsTaskData::PercentDone
                      << nsTaskData::Cost
                      << nsTaskData::Risk;
    connect(m_prefs, SIGNAL(shortcutChanged(Actions::Actions,QKeySequence)),
                       SLOT(shortcutChanged(Actions::Actions,QKeySequence)));
}

PrefsManager::~PrefsManager()
{
    delete m_prefs;
}

bool PrefsManager::isColumnSelected(nsTaskData::TaskDataMember member) const
{
    return m_selectedColumns.contains(member);
}

void PrefsManager::init(IToolManager *manager)
{
    m_manager = manager;
    addAction(Actions::EditPreferences);
}

const char * PrefsManager::getActionSlot(Actions::Actions action) const
{
    if (action == Actions::EditPreferences)
        return SLOT(showPreferences());
    return 0;
}

QObject * PrefsManager::getReciever()
{
    return this;
}

bool PrefsManager::isActionEnabled(Actions::Actions action) const
{
    if (action == Actions::EditPreferences)
        return true;
    return false;
}

void PrefsManager::showPreferences()
{
    PrefsDialog dialog(m_parentWindow, m_prefs, m_uiManager);
    dialog.exec();
}

bool PrefsManager::saveOnMinimize() const
{
    return m_prefs->saveOnMinimize();
}

bool PrefsManager::saveOnExit() const
{
    return m_prefs->saveOnExit();
}

void PrefsManager::setUiManager(IUiManager *uiManager)
{
    m_uiManager = uiManager;
}

void PrefsManager::shortcutChanged(Actions::Actions action, QKeySequence sequence)
{
    m_uiManager->setActionShortcut(action, sequence);
}

QKeySequence PrefsManager::shortcutForAction(Actions::Actions id) const
{
    return m_prefs->shortcutForAction(id);
}
