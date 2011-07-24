#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include "ipreferences.h"

#include <QSet>

class PrefsManager : public IPreferences
{
public:
    PrefsManager();
public:
    virtual bool isColumnSelected(nsTaskData::TaskDataMember member) const;
private:
    QSet<nsTaskData::TaskDataMember> m_selectedColumns;
};

#endif // PREFSMANAGER_H
