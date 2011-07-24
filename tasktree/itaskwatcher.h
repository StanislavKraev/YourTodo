#ifndef ITASKWATCHER_H
#define ITASKWATCHER_H

#include "tasktree/treecolumndata.h"

class Task;

class ITaskWatcher
{
protected:
    ITaskWatcher() {}
    ~ITaskWatcher() {}
private:
    ITaskWatcher(const ITaskWatcher&);
    ITaskWatcher& operator=(const ITaskWatcher&);
public:
    virtual void taskChanged(nsTaskData::TaskDataMember member, Task* task) = 0;
};

#endif // ITASKWATCHER_H
