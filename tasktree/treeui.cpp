#include <QTreeView>
#include <QHeaderView>
#include <QApplication>

#include "delegates.h"

#include "treeui.h"

TreeUi::TreeUi(const QFont &strikedOutFont, QTreeView *view) :
    m_strikedOutFont(strikedOutFont),
    m_view(view)
{
}

int TreeUi::columnsCount() const
{
    return m_columns.count();
}

QString TreeUi::headerTitle(int section) const
{
    if (section >= 0 && section <= m_columns.count() - 1)
        return m_columns[section].title;
    return QString();
}

QVariant TreeUi::itemData(int column, Task::Ptr task) const
{
    if (column >= 0 && column <= m_columns.count() - 1)
    {
        TaskDataMember member = m_columns[column].taskDataMember;
        switch (member)
        {
        case Title:
            return task->title();
        case Priority:
            if (task->percentDone() >= 100)
                return QVariant();
            return task->priority();
        case PercentDone:
            if (task->percentDone() >= 100)
                return QVariant();
            return QString("%1%").arg(task->percentDone());
        case IconIndex:
            return task->iconIndex();
        case Position:
            return task->posAttr();
        case Risk:
            return task->risk();
        case Cost:
            return task->calcCost();
        case StartDate:
            return task->startDate();
        case DoneDate:
            return task->doneDate();
        case CreationDate:
            return task->creationDate();
        case LastModified:
            return task->lastModDate();
        case CommentsType:
            return task->commentsType();
        case Comments:
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
    if (column >= 0 && column <= m_columns.count() - 1)
    {
        TaskDataMember member = m_columns[column].taskDataMember;
        switch (member)
        {
        case Title:
            task->setTitle(data.toString());
        }
    }
}

QFont TreeUi::strikedOutFont() const
{
    return m_strikedOutFont;
}

QFont TreeUi::font(int column, Task::Ptr task) const
{
    if (column >= 0 && column <= m_columns.count() - 1)
    {
        TaskDataMember member = m_columns[column].taskDataMember;
        if (member == Title)
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
    if (task && column >= 0 && column <= m_columns.count() - 1)
    {
        TaskDataMember member = m_columns[column].taskDataMember;
        switch (member)
        {
            case Priority:
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
    int index = 0;
    foreach(const TreeColumnData &column, m_columns)
    {
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
        index++;
    }
}

QBrush TreeUi::background(int column, Task::Ptr task) const
{
    if (task && column >= 0 && column <= m_columns.count() - 1)
    {
        TaskDataMember member = m_columns[column].taskDataMember;
        switch (member)
        {
        case Priority:
            return task->priorityColor();
        }
    }
    return Qt::white;
}
