#include <QTreeView>
#include <QHeaderView>
#include <QApplication>

#include "delegates.h"
#include "ipreferences.h"

#include "treeui.h"

TreeUi::TreeUi(const QFont &strikedOutFont, QTreeView *view, IPreferences* prefs) :
    m_strikedOutFont(strikedOutFont),
    m_view(view),
    m_prefs(prefs)
{
}

int TreeUi::columnsCount() const
{
    int count = 0;
    foreach(const TreeColumnData &column, m_columns)
    {
        if (m_prefs->isColumnSelected(column.taskDataMember))
            count++;
    }

    return count;
}

QString TreeUi::headerTitle(int section) const
{
    if (section >= 0 && section <= columnsCount() - 1)
        return columnData(section).title;
    return QString();
}

QVariant TreeUi::itemData(int column, Task::Ptr task) const
{
    if (column >= 0 && column <= columnsCount() - 1)
    {
        nsTaskData::TaskDataMember member = columnData(column).taskDataMember;
        switch (member)
        {
        case nsTaskData::Title:
            return task->title();
        case nsTaskData::Priority:
            if (task->percentDone() >= 100)
                return QVariant();
            return task->priority();
        case nsTaskData::PercentDone:
            if (task->percentDone() >= 100)
                return QVariant();
            return QString("%1%").arg(task->percentDone());
        case nsTaskData::IconIndex:
            return task->iconIndex();
        case nsTaskData::Position:
            return task->posAttr();
        case nsTaskData::Risk:
            return task->risk();
        case nsTaskData::Cost:
            return task->calcCost();
        case nsTaskData::StartDate:
            return task->startDate();
        case nsTaskData::DoneDate:
            return task->doneDate();
        case nsTaskData::CreationDate:
            return task->creationDate();
        case nsTaskData::LastModified:
            return task->lastModDate();
        case nsTaskData::CommentsType:
            return task->commentsType();
        case nsTaskData::Comments:
            return task->comments();
        }
    }
    return QVariant();
}

void TreeUi::addColumn(TreeColumnData columnData)
{
    m_columns.append(columnData);
}

void TreeUi::removeColumns()
{
    m_columns.clear();
}

void TreeUi::updateData(Task::Ptr task, int column, QVariant data)
{
    if (column >= 0 && column <= columnsCount() - 1)
    {
        nsTaskData::TaskDataMember member = columnData(column).taskDataMember;
        switch (member)
        {
        case nsTaskData::Title:
            task->setTitle(data.toString());
            break;
        case nsTaskData::Cost:
            task->setCost(data.toDouble());
            break;
        case nsTaskData::PercentDone:
            task->setPercentDone(data.toInt());
            break;
        }
    }
}

QFont TreeUi::strikedOutFont() const
{
    return m_strikedOutFont;
}

QFont TreeUi::font(int column, Task::Ptr task) const
{
    if (column >= 0 && column <= columnsCount() - 1)
    {
        nsTaskData::TaskDataMember member = columnData(column).taskDataMember;
        if (member == nsTaskData::Title)
        {
            QFont font;
            if (task->percentDone() >= 100)
            {
                font = strikedOutFont();
            }
            else if (!task->parent())
                font.setBold(true);
            return font;
        }
    }
    return QFont();
}

QBrush TreeUi::foreground(int column, Task::Ptr task) const
{
    if (task && column >= 0 && column <= columnsCount() - 1)
    {
        nsTaskData::TaskDataMember member = columnData(column).taskDataMember;
        switch (member)
        {
            case nsTaskData::Priority:
            {
                if (task->priority() > 3)
                    return Qt::white;
                else
                    return Qt::black;
            }
        }
    }
    return Qt::black;
}

void TreeUi::init()
{
    int count = columnsCount();
    for (int index = 0; index < count; ++index)
    {
        TreeColumnData column = columnData(index);
        if (column.colType == TreeColumnData::PRIORITY)
            m_view->setItemDelegateForColumn(index, new PriorityDelegate(m_view));
        else if (column.colType == TreeColumnData::ICONINDEX)
            m_view->setItemDelegateForColumn(index, new IconIndexDelegate(m_view));
        if (column.width < 0)
        {
            m_view->header()->setResizeMode(index, QHeaderView::ResizeToContents);
        }
        else if (column.width > 0)
        {
            m_view->header()->resizeSection(index, column.width);
            m_view->header()->setResizeMode(index, QHeaderView::Fixed);
        }
    }
}

QBrush TreeUi::background(int column, Task::Ptr task) const
{
    if (task && column >= 0 && column <= columnsCount() - 1)
    {
        nsTaskData::TaskDataMember member = columnData(column).taskDataMember;
        switch (member)
        {
        case nsTaskData::Priority:
            return task->priorityColor();
        }
    }
    return Qt::white;
}

TreeColumnData TreeUi::columnData(int column) const
{
    if (column >= 0 && column <  columnsCount())
    {
        int i = -1;
        foreach (const TreeColumnData &data, m_columns)
        {
            if (m_prefs->isColumnSelected(data.taskDataMember))
            {
                ++i;
                if (i == column)
                    return data;
            }
        }
    }
    return TreeColumnData();
}
