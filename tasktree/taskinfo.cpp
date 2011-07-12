#include "taskinfo.h"

QString color2str(QColor color)
{
    int i = color.red() + color.green() << 8 + color.blue() << 16;
    return QString("%1").arg(i);
}

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

TaskInfo TaskInfo::fromTaskPtr(Task::Ptr task)
{
    TaskInfo info;
    info.comments = task->comments();
    info.commentsType = task->commentsType();
    info.cost = task->cost();
    info.creationDate = task->creationDate().toMSecsSinceEpoch();
    info.doneDate = task->doneDate().toMSecsSinceEpoch();
    info.iconIndex = task->iconIndex();
    info.id = task->id();
    info.lastMod = task->lastModDate().toMSecsSinceEpoch();
    info.parentId = task->parent()->id();
    info.percentDone = task->percentDone();
    info.pos = task->posAttr();
    info.priority = task->priority();
    info.priorityColor = color2str(task->priorityColor());
    info.risk = task->risk();
    info.startDate = task->startDate().toMSecsSinceEpoch();
    info.textColor = color2str(task->textColor());
    info.title = task->title();

    return info;
}
