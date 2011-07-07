#include <QBrush>
#include <QFont>

#include "tasktree/itaskstorage.h"
#include "tasktree/itreeuiprovider.h"

#include "treemodel.h"


TreeModel::TreeModel(QObject *parent, ITaskStorage *taskStorage, ITreeUiProvider *treeUi) :
    QAbstractItemModel(parent),
    m_taskStorage(taskStorage),
    m_treeUi(treeUi)
{
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    return m_treeUi->columnsCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (!item)
            return QVariant();
        return m_treeUi->itemData(index.column(), item);
    }
    else if (role == Qt::FontRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (!item)
            return QVariant();
        return m_treeUi->font(index.column(), item);
    }
    else if (role == Qt::ForegroundRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (!item)
            return QVariant();
        if (item->percentDone() >= 100)
        {
            return Qt::darkCyan;
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        if (index.row() % 2)
            return QVariant(Qt::red);
    }
    else if (role == Qt::UserRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
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

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
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
        return createIndex(row, column, m_taskStorage->getAt(row)->id());
    else
        parentItem = m_taskStorage->getById(parent.internalId());

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

    Task::Ptr childItem = m_taskStorage->getById(index.internalId());
    if (!childItem)
        return QModelIndex();

    Task::Ptr parentItem = childItem->parent();
    if (!parentItem || parentItem->id() == -1)
        return QModelIndex();

    Task::Ptr grandItem = parentItem->parent();
    int row = grandItem ? grandItem->pos(parentItem) :
                          m_taskStorage->pos(parentItem);

    return createIndex(row, 0, parentItem->id());
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    Task::Ptr parentItem;

    if (!parent.isValid())
        return m_taskStorage->count();
    else
        parentItem = m_taskStorage->getById(parent.internalId());

    return parentItem ? parentItem->count() : 0;
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (item)
        {
            m_treeUi->updateData(item, index.column(), value);
            emit(dataChanged(index, index));
            return true;
        }
    }
    else if (role == Qt::UserRole)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (item)
        {
            Task::Ptr newItem = value.value<Task::Ptr>();
            m_taskStorage->replace(item, newItem);
        }
    }
    else if (role == Qt::UserRole + 1)
    {
        Task::Ptr item = m_taskStorage->getById(index.internalId());
        if (item)
        {
            unsigned short curData = item->percentDone();
            item->setPercentDone(curData == 0 ? 100 : 0);
            emit(dataChanged(index, createIndex(index.row(), m_treeUi->columnsCount() - 1, (int)index.internalId())));
            return true;
        }
    }
    return false;
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Task::Ptr task = parent.isValid() ?
        m_taskStorage->getById(parent.internalId()) :
        m_taskStorage->root();
    if (task)
    {
        emit(beginInsertRows(parent, row, row + count - 1));
        bool result = m_taskStorage->insertNewTasks(task, row, count);
        emit(endInsertRows());
        return result;
    }
    return false;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Task::Ptr task = parent.isValid() ?
        m_taskStorage->getById(parent.internalId()) :
        m_taskStorage->root();
    if (task)
    {
        emit(beginRemoveRows(parent, row, row + count - 1));
        bool result = m_taskStorage->removeTasks(task, row, count);
        emit(endRemoveRows());
        return result;
    }
    return false;
}
