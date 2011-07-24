#include "prefsmanager.h"

PrefsManager::PrefsManager()
{
    m_selectedColumns << Title
                      << Priority
                      << IconIndex
                      << PercentDone;
}

bool PrefsManager::isColumnSelected(TaskDataMember member) const
{
    return m_selectedColumns.contains(member);
}
