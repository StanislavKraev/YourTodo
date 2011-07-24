#include "treecolumndata.h"

TreeColumnData::TreeColumnData(QString _title, nsTaskData::TaskDataMember _taskDataMember,
                               int _width, ColumnType type) :
    title(_title), taskDataMember(_taskDataMember),
    width(_width), colType(type)
{
}

TreeColumnData::TreeColumnData() : taskDataMember(nsTaskData::InvalidTaskMember),
    width(-1), colType(GENERAL)
{

}
