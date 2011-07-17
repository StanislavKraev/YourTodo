#include "treecolumndata.h"

TreeColumnData::TreeColumnData(QString _title, TaskDataMember _taskDataMember,
                               int _width, ColumnType type) :
    title(_title), taskDataMember(_taskDataMember),
    width(_width), colType(type)
{
}
