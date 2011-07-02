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

QVariant TreeUi::itemData(int column, Task::Ptr task)
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
        case Comment:
            //task->setComment(data.toString());
            break;
        }
    }
}

QFont TreeUi::strikedOutFont() const
{
    return m_strikedOutFont;
}
