#ifndef TREECOLUMNDATA_H
#define TREECOLUMNDATA_H

#include <QString>

namespace nsTaskData
{
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
}

struct TreeColumnData
{
    enum ColumnType
    {
        GENERAL = 0,
        PRIORITY,
        ICONINDEX
    };

    QString title;
    nsTaskData::TaskDataMember taskDataMember;
    ColumnType colType;
    int width;
public:
    TreeColumnData();
    TreeColumnData(QString _title, nsTaskData::TaskDataMember _taskDataMember,
                   int _width = 0, ColumnType type = GENERAL);
};

#endif // TREECOLUMNDATA_H
