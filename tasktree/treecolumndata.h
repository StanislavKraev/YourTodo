#ifndef TREECOLUMNDATA_H
#define TREECOLUMNDATA_H

#include <QString>

enum TaskDataMember
{
    Title = 1,
    Comment,
    Priority,
    PercentDone,
    IconIndex
};

struct TreeColumnData
{
    QString title;
    TaskDataMember taskDataMember;
public:
    TreeColumnData(QString _title, TaskDataMember _taskDataMember);
};

#endif // TREECOLUMNDATA_H
