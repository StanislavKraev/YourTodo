#include "treecolumndata.h"

TreeColumnData::TreeColumnData(QString _title, TaskDataMember _taskDataMember, bool visible,
                               int _width, ColumnType type) :
    title(_title), taskDataMember(_taskDataMember),
    width(_width), colType(type), visible(visible)
{
}

TreeColumnData::TreeColumnData() : taskDataMember(InvalidTaskMember),
    width(-1), colType(GENERAL), visible(false)
{

}
