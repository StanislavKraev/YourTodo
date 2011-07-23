#ifndef TREECOLUMNDATA_H
#define TREECOLUMNDATA_H

#include <QString>

enum TaskDataMember
{
    InvalidTaskMember = 0,
    Title = 1,
    Priority,
    PercentDone,
    IconIndex,
    Position,
    Risk,
    Cost,
    StartDate,
    DoneDate,
    CreationDate,
    LastModified,
    CommentsType,
    Comments
};

struct TreeColumnData
{
    enum ColumnType
    {
        GENERAL = 0,
        PRIORITY,
        ICONINDEX
    };

    QString title;
    TaskDataMember taskDataMember;
    ColumnType colType;
    int width;
    bool visible;
public:
    TreeColumnData();
    TreeColumnData(QString _title, TaskDataMember _taskDataMember, bool visible = true,
                   int _width = 0, ColumnType type = GENERAL);
};

#endif // TREECOLUMNDATA_H
