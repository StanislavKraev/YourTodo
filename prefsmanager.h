#ifndef PREFSMANAGER_H
#define PREFSMANAGER_H

#include "ipreferences.h"

#include <QSet>

class PrefsManager : public IPreferences
{
public:
    PrefsManager();
public:
    virtual bool isColumnSelected(TaskDataMember member) const;
private:
    QSet<TaskDataMember> m_selectedColumns;
};

#endif // PREFSMANAGER_H
