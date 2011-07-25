#ifndef ITASKWATCHER_H
#define ITASKWATCHER_H

#include "tasktree/task.h"


class ITaskWatcher
{
protected:
    ITaskWatcher() {}
    ~ITaskWatcher() {}
private:
    ITaskWatcher(const ITaskWatcher&);
    ITaskWatcher& operator=(const ITaskWatcher&);
public:
    virtual void taskChanged(nsTaskData::TaskDataMember member, Task::Ptr task) = 0;
};

#endif // ITASKWATCHER_H
