#include "utils.h"

#include "taskinfo.h"

QString color2str(QColor color)
{
    int i = color.red() + (color.green() << 8) + (color.blue() << 16);
    return QString("%1").arg(i);
}

Task::Ptr TaskInfo::toTaskPtr() const
{
    Task::CommentsType commType = Task::UNDEFINED;
    if (commentsType == "PLAIN_TEXT")
        commType = Task::PLAIN_TEXT;
    Task::Ptr task = Task::Ptr(
                new Task(id,
                         title,
                         percentDone,
                         comments,
                         commType,
                         cost,
                         fromOleTime(creationDate),
                         fromOleTime(doneDate),
                         iconIndex,
                         fromOleTime(lastMod),
                         pos,
                         priority,
                         fromRawColor(priorityColor),
                         risk,
                         fromOleTime(startDate),
                         fromRawColor(textColor))
                );
    return task;
}

TaskInfo TaskInfo::fromTaskPtr(Task::Ptr task)
{
    TaskInfo info;
    info.comments = task->comments();

    info.commentsType = "";
    switch (task->commentsType())
    {
    case Task::PLAIN_TEXT:
        info.commentsType = "PLAIN_TEXT";
    default:
        break;
    }

    info.cost = task->cost();
    info.calcCost = task->calcCost();
    info.creationDate = toOleTime(task->creationDate());
    info.doneDate = toOleTime(task->doneDate());
    info.iconIndex = task->iconIndex();
    info.id = task->id();
    info.lastMod = toOleTime(task->lastModDate());
    info.percentDone = task->percentDone();
    info.pos = task->calcPosAttr();
    info.priority = task->priority();
    info.priorityColor = toRawColor(task->priorityColor());
    info.risk = task->risk();
    info.startDate = toOleTime(task->startDate());
    info.textColor = toRawColor(task->textColor());
    info.title = task->title();

    return info;
}
