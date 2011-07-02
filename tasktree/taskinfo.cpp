#include "taskinfo.h"

Task::Ptr TaskInfo::toTaskPtr() const
{
    Task::Ptr task = Task::Ptr(
                new Task(id,
                         title,
                         percentDone,
                         comments,
                         commentsType,
                         cost,
                         creationDate,
                         doneDate,
                         iconIndex,
                         lastMod,
                         pos,
                         priority,
                         priorityColor,
                         risk,
                         startDate,
                         textColor)
                );
    return task;
}
