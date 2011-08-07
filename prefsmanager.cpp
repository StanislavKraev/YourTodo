#include "prefsmanager.h"

PrefsManager::PrefsManager()
{
    m_selectedColumns << nsTaskData::Title
                      << nsTaskData::Priority
                      << nsTaskData::IconIndex
                      << nsTaskData::PercentDone
                      << nsTaskData::Cost;
}

bool PrefsManager::isColumnSelected(nsTaskData::TaskDataMember member) const
{
    return m_selectedColumns.contains(member);
}
