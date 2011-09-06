#ifndef IPREFERENCES_H
#define IPREFERENCES_H

#include <QKeySequence>

#include "tasktree/treecolumndata.h"
#include "actionids.h"

class IPreferences
{
protected:
    IPreferences() {}
    ~IPreferences() {}
private:
    IPreferences(const IPreferences&);
    IPreferences& operator=(const IPreferences&);
public:
    virtual bool isColumnSelected(nsTaskData::TaskDataMember member) const = 0;
    virtual bool saveOnMinimize() const = 0;
    virtual bool saveOnExit() const = 0;
    virtual QKeySequence shortcutForAction(Actions::Actions id) const = 0;
};

#endif // IPREFERENCES_H
