#include "prefsmanager.h"

PrefsManager::PrefsManager()
{
    m_selectedColumns << Title
                      << Priority
                      << PercentDone
                      << IconIndex
                      << PercentDone;
}

bool PrefsManager::isColumnSelected(TaskDataMember member) const
{
    return m_selectedColumns.contains(member);
}
