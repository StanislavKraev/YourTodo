#include "treecolumndata.h"

TreeColumnData::TreeColumnData(QString _title, TaskDataMember _taskDataMember, ColumnType type) :
    title(_title), taskDataMember(_taskDataMember), colType(type)
{
}
