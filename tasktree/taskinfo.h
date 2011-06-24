#ifndef TASKINFO_H
#define TASKINFO_H

#include "task.h"

struct TaskInfo
{
    int id;
    QString title;
    int iconIndex;
    QString comments;
    int pos;
    int priority;
    int risk;
    unsigned short percentDone;
    double cost;
    double doneDate;
    double startDate;
    double creationDate;
    double lastMod;
    QString commentsType;
    QString textColor;
    QString priorityColor;
    int parentId;
public:
    Task::Ptr toTaskPtr() const;
};

#endif // TASKINFO_H
