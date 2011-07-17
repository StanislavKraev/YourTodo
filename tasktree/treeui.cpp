#include <QTreeView>
#include <QHeaderView>
#include <QApplication>
#include <QStyledItemDelegate>

#include "treeui.h"

class PriorityDelegate : public QStyledItemDelegate
{
public:
    virtual void paint(QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        // TODO: if percent_done >= 100% - do not draw anything (or just background)
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        QStyle *style = QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
    }
};

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
            m_view->setItemDelegateForColumn(index, new PriorityDelegate());
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
