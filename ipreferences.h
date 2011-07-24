#ifndef IPREFERENCES_H
#define IPREFERENCES_H

#include "tasktree/treecolumndata.h"

class IPreferences
{
protected:
    IPreferences() {}
    ~IPreferences() {}
private:
    IPreferences(const IPreferences&);
    IPreferences& operator=(const IPreferences&);
public:
    virtual bool isColumnSelected(TaskDataMember member) const = 0;
};

#endif // IPREFERENCES_H