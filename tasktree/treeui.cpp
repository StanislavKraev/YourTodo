#include "treeui.h"

TreeUi::TreeUi(const QFont &strikedOutFont) : m_strikedOutFont(strikedOutFont)
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
        case Comment:
            return task->comments();
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
