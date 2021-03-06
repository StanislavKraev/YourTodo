#include <QBrush>
#include <QFont>

#include "tasktree/itasklist.h"
#include "tasktree/itreeuiprovider.h"

#include "treemodel.h"


TreeModel::TreeModel(QObject *parent, ITaskList *taskList, ITreeUiProvider *treeUi) :
    QAbstractItemModel(parent),
    m_taskList(taskList),
    m_treeUi(treeUi)
{
    m_taskList->addWatch(this);
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return m_treeUi->columnsCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::UserRole + 2)
    {
        return m_treeUi->column(nsTaskData::Title);
    }

    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::ToolTipRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (!item)
            return QVariant();
        return m_treeUi->itemData(index.column(), item);
    }
    else if (role == Qt::FontRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (!item)
            return QVariant();
        return m_treeUi->font(index.column(), item);
    }
    else if (role == Qt::ForegroundRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        return m_treeUi->foreground(index.column(), item);
    }
    else if (role == Qt::BackgroundRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        return m_treeUi->background(index.column(), item);
    }
    else if (role == Qt::UserRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (!item)
            return QVariant();
        return qVariantFromValue(item);
    }
    return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    int titleIndex = m_treeUi->column(nsTaskData::Title);
    if ((titleIndex >= 0) && (index.column() == titleIndex))
        flags |= Qt::ItemIsEditable;

    return flags;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_treeUi->headerTitle(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Task::Ptr parentItem;

    if (!parent.isValid())
        return createIndex(row, column, m_taskList->getAt(row)->id());
    else
        parentItem = m_taskList->getById(parent.internalId());

    Task::Ptr childItem = parentItem->getAt(row);
    if (childItem)
        return createIndex(row, column, childItem->id());
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Task::Ptr childItem = m_taskList->getById(index.internalId());
    if (!childItem)
        return QModelIndex();

    Task::Ptr parentItem = childItem->parent();
    if (!parentItem || parentItem->id() == -1)
        return QModelIndex();

    Task::Ptr grandItem = parentItem->parent();
    int row = grandItem ? grandItem->pos(parentItem) :
                          m_taskList->pos(parentItem);

    return createIndex(row, 0, parentItem->id());
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    Task::Ptr parentItem;

    if (!parent.isValid())
        return m_taskList->count();
    else
        parentItem = m_taskList->getById(parent.internalId());

    return parentItem ? parentItem->count() : 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (item)
        {
            m_treeUi->updateData(item, index.column(), value);
            emit(dataChanged(index, index));
            return true;
        }
    }
    else if (role == Qt::UserRole)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (item)
        {
            Task::Ptr newItem = value.value<Task::Ptr>();
            m_taskList->replace(item, newItem);
        }
    }
    else if (role == Qt::UserRole + 3)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (item)
        {
            Task::Ptr newItem = value.value<Task::Ptr>();
            item->setPriority(newItem->priority());
        }
    }
    else if (role == Qt::UserRole + 1)
    {
        Task::Ptr item = m_taskList->getById(index.internalId());
        if (item)
        {
            unsigned short curData = item->percentDone();
            item->toggleDone(curData < 100 ? 100 : 0);
            emit(dataChanged(index, createIndex(index.row(), m_treeUi->columnsCount() - 1, (int)index.internalId())));
            return true;
        }
    }
    return false;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Task::Ptr task = parent.isValid() ?
        m_taskList->getById(parent.internalId()) :
        m_taskList->root();
    if (task)
    {
        emit(beginInsertRows(parent, row, row + count - 1));
        bool result = m_taskList->insertNewTasks(task, row, count);
        emit(endInsertRows());
        return result;
    }
    return false;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Task::Ptr task = parent.isValid() ?
        m_taskList->getById(parent.internalId()) :
        m_taskList->root();
    if (task)
    {
        emit(beginRemoveRows(parent, row, row + count - 1));
        bool result = m_taskList->removeTasks(task, row, count);
        emit(endRemoveRows());
        return result;
    }
    return false;
}

void TreeModel::taskChanged(nsTaskData::TaskDataMember member, Task* task)
{
    (void)member;
    QModelIndex id = idFromPtr(task);

    if (id.isValid())
    {
        QModelIndex idRight = createIndex(id.row(), m_treeUi->columnsCount() - 1, (int)id.internalId());
        emit(dataChanged(id, idRight));
    }
}

QModelIndex TreeModel::idFromPtr(Task* task, QModelIndex parent) const
{
    int count = rowCount(parent);
    for (int i = 0; i < count; ++i)
    {
        QModelIndex id = index(i, 0, parent);
        if (id.isValid())
        {
            Task::Ptr testTask = m_taskList->getById(id.internalId());
            if (&(*testTask) == task)
                return id;

            QModelIndex childrenTry = idFromPtr(task, id);
            if (childrenTry.isValid())
                return childrenTry;
        }
    }
    return QModelIndex();
}
