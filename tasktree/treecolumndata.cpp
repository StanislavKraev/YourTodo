#include "treecolumndata.h"

TreeColumnData::TreeColumnData(QString _title, nsTaskData::TaskDataMember _taskDataMember,
                               int _width, ColumnType type) :
    title(_title), taskDataMember(_taskDataMember),
    colType(type), width(_width)
{
}

TreeColumnData::TreeColumnData() : taskDataMember(nsTaskData::InvalidTaskMember),
    colType(GENERAL), width(-1)
{

}
