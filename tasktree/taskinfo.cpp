#include "taskinfo.h"

Task::Ptr TaskInfo::toTaskPtr() const
{
    Task::Ptr task = Task::Ptr(new Task(id, title, percentDone));
    return task;
}
