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
    enum ColumnType
    {
        GENERAL = 0,
        PRIORITY
    };

    QString title;
    TaskDataMember taskDataMember;
    ColumnType colType;
public:
    TreeColumnData(QString _title, TaskDataMember _taskDataMember, ColumnType type = GENERAL);
};

#endif // TREECOLUMNDATA_H
