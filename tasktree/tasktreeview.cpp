#include <QMap>

#include "tasktree/task.h"

#include "tasktreeview.h"

class StoreSelectionHelper
{
    QItemSelectionModel *m_selectionModel;
    QAbstractItemModel *m_itemModel;
    QList<int> m_selectedValues;
    int m_curIndex;
    QTreeView *m_view;
public:
    StoreSelectionHelper(QItemSelectionModel *selectionModel,
                         QAbstractItemModel *itemModel,
                         QTreeView *view) :
        m_selectionModel(selectionModel),
        m_itemModel(itemModel),
        m_curIndex(-1),
        m_view(view)
    {
        QModelIndexList selections = m_selectionModel->selectedRows();
        foreach (QModelIndex id, selections)
        {
            Task::Ptr val = m_itemModel->data(id, Qt::UserRole).value<Task::Ptr>();
            m_selectedValues.append(val->id());
        }
        QModelIndex current = m_selectionModel->currentIndex();
        m_curIndex = (m_itemModel->data(current, Qt::UserRole).value<Task::Ptr>())->id();
    }
    void RestoreSelection(const QModelIndex &id)
    {
        int val = (m_itemModel->data(id, Qt::UserRole).value<Task::Ptr>())->id();
        if (m_curIndex == val)
        {
            m_selectionModel->setCurrentIndex(id, QItemSelectionModel::NoUpdate);
            m_view->scrollTo(id);
        }
        foreach (int newVal, m_selectedValues)
        {
            if (newVal == val)
                m_selectionModel->select(id, QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
        int rowCount = m_itemModel->rowCount(id);
        for (int row = 0; row < rowCount; ++row)
        {
            QModelIndex childId = m_itemModel->index(row, 0, id);
            RestoreSelection(childId);
        }
    }

    ~StoreSelectionHelper()
    {
        m_selectionModel->clearSelection();
        int rowCount = m_itemModel->rowCount();
        for (int row = 0; row < rowCount; ++row)
        {
            QModelIndex id = m_itemModel->index(row, 0);
            RestoreSelection(id);
        }
    }
};

TaskTreeView::TaskTreeView(QWidget *parent) :
    QTreeView(parent)
{
}

void TaskTreeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space &&
        event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier))
    {
        toggleSelectedTasks();
        event->accept();
    }
    else if (event->key() == Qt::Key_N &&
             event->modifiers() == Qt::ControlModifier)
    {
        addTaskBelowCursor();
        event->accept();
    }
    else if (event->key() == Qt::Key_Delete &&
             event->modifiers() == Qt::NoModifier)
    {
        removeSelectedTasks();
        event->accept();
    }
    else if (event->key() == Qt::Key_Right &&
             event->modifiers() == Qt::ControlModifier)
    {
        shiftSelectedTasksRight();
        event->accept();
    }
    else if (event->key() == Qt::Key_Left &&
             event->modifiers() == Qt::ControlModifier)
    {
        shiftSelectedTasksLeft();
        event->accept();
    }
    else if (event->key() == Qt::Key_Up &&
             event->modifiers() == Qt::ControlModifier)
    {
        shiftSelectedTasksUp();
        event->accept();
    }
    else if (event->key() == Qt::Key_Down &&
             event->modifiers() == Qt::ControlModifier)
    {
        shiftSelectedTasksDown();
        event->accept();
    }
    else
        QTreeView::keyPressEvent(event);
}

void TaskTreeView::toggleSelectedTasks()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    foreach (QModelIndex index, selectedList)
    {
        model()->setData(index, QVariant(), Qt::UserRole + 1); // toggle percent done
    }
}

void TaskTreeView::addTaskBelowCursor()
{
    int row(0);
    QModelIndex parent;

    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
    {
        if (model()->rowCount() > 0)
            return;
        row = 0;
    }
    else
    {
        QModelIndex selection = selectedList.first();
        row = selection.row() + 1;
        parent = selection.parent();
    }
    model()->insertRows(row, 1, parent);
    int titleColumn = model()->data(QModelIndex(), Qt::UserRole + 2).toInt();
    QModelIndex newItemIndex = model()->index(row, titleColumn, parent);
    selectionModel()->select(newItemIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    setCurrentIndex(newItemIndex);
    edit(newItemIndex);
}

void TaskTreeView::removeSelectedTasks()
{
    QModelIndexList selectedList = selectionModel()->selectedRows();
    while (selectedList.count())
    {
        QMap<Task::Ptr, QModelIndex> taskMap;
        foreach (QModelIndex id, selectedList)
        {
            QVariant taskData = model()->data(id, Qt::UserRole);
            Task::Ptr task = taskData.value<Task::Ptr>();
            taskMap[task] = id;
        }

        foreach (Task::Ptr task, taskMap.keys())
        {
            Task::Ptr parent = task->parent();
            bool parentSelected(false);
            while (parent && !parentSelected)
            {
                parent = parent->parent();
                parentSelected = taskMap.contains(parent);
            }
            if (!parentSelected)
            {
                QModelIndex taskIndex = taskMap[task];
                model()->removeRows(taskIndex.row(), 1, taskIndex.parent());
                break;
            }
        }
        selectedList = selectionModel()->selectedRows();
    }
    // TODO: select next element after last removed.
}

void TaskTreeView::getSelectedRowsRange(const QModelIndexList &selectedList, int &startRow, int &endRow) const
{
    startRow = 100000000;
    endRow = -1;
    foreach (QModelIndex index, selectedList)
    {
        if (index.row() > endRow)
            endRow = index.row();
        if (index.row() < startRow)
            startRow = index.row();
    }
}

void TaskTreeView::shiftSelectedTasksRight()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    QModelIndex firstItem = getFirstSelection(selectedList);
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    if (firstItem.row() < 1)
        return;

    selectedList.removeOne(firstItem);
    if (selectedList.count())
    {
        if (!checkAllAreChildren(mainParent, selectedList))
            return;

        QModelIndexList topLevelChildren;
        getItemChildren(mainParent, selectedList, topLevelChildren);
        itemsToMove.append(topLevelChildren);
    }
    if (itemsToMove.count())
    {
        StoreSelectionHelper selHelper(selectionModel(), model(), this);
        QModelIndex newParent = model()->index(firstItem.row() - 1, 0, mainParent);
        if (!newParent.isValid())
            return;

        int startRow = model()->rowCount(newParent);
        int curRow = model()->rowCount(newParent);
        model()->insertRows(startRow, itemsToMove.count(), newParent);
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, newParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
        expand(model()->index(newParent.row(), 0, mainParent));

        selectionModel()->setCurrentIndex(model()->index(startRow, 0, newParent),
                                          QItemSelectionModel::NoUpdate);
    }
}

void TaskTreeView::shiftSelectedTasksLeft()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    QModelIndex firstItem = getFirstSelection(selectedList);
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    if (!mainParent.isValid())
        return;

    selectedList.removeOne(firstItem);
    if (selectedList.count())
    {
        if (!checkAllAreChildren(mainParent, selectedList))
            return;

        QModelIndexList topLevelChildren;
        getItemChildren(mainParent, selectedList, topLevelChildren);
        itemsToMove.append(topLevelChildren);
    }
    if (itemsToMove.count())
    {
        StoreSelectionHelper selHelper(selectionModel(), model(), this);
        QModelIndex newParent = mainParent.parent();
        int startRow = mainParent.row() + 1;
        int curRow = mainParent.row() + 1;
        model()->insertRows(startRow, itemsToMove.count(), newParent);
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, newParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
        selectionModel()->setCurrentIndex(model()->index(startRow, 0, newParent),
                                          QItemSelectionModel::NoUpdate);
    }
}

void TaskTreeView::shiftSelectedTasksUp()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    QModelIndex firstItem = getFirstSelection(selectedList);
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    if (firstItem.row() < 1)
        return;

    selectedList.removeOne(firstItem);
    if (selectedList.count())
    {
        if (!checkAllAreChildren(mainParent, selectedList))
            return;

        QModelIndexList topLevelChildren;
        getItemChildren(mainParent, selectedList, topLevelChildren);
        itemsToMove.append(topLevelChildren);
    }
    if (itemsToMove.count())
    {
        StoreSelectionHelper selHelper(selectionModel(), model(), this);
        int startRow = firstItem.row() - 1;
        const int count = itemsToMove.count();
        int curRow = startRow;
        model()->insertRows(startRow, count, mainParent);
        int least = count;
        while (least)
        {
            int curRowOld = curRow + count + 1;
            QModelIndex newItemId = model()->index(curRow, 0, mainParent);
            QModelIndex oldItemId = model()->index(curRowOld, 0, mainParent);
            QVariant val = model()->data(oldItemId, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;

            least--;
        }
        model()->removeRows(startRow + count + 1, count, mainParent);
    }
}

void TaskTreeView::shiftSelectedTasksDown()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    QModelIndex firstItem = getFirstSelection(selectedList);
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    int parentChildrenCount = model()->rowCount(mainParent);

    selectedList.removeOne(firstItem);
    if (selectedList.count())
    {
        if (!checkAllAreChildren(mainParent, selectedList))
            return;

        QModelIndexList topLevelChildren;
        getItemChildren(mainParent, selectedList, topLevelChildren);
        itemsToMove.append(topLevelChildren);
    }

    const int count = itemsToMove.count();
    if (firstItem.row() + count >= parentChildrenCount)
        return;

    if (count)
    {
        StoreSelectionHelper selHelper(selectionModel(), model(), this);
        int startRow = firstItem.row() + count + 1;
        int curRow = startRow;
        model()->insertRows(startRow, count, mainParent);
        int least = count;
        while (least)
        {
            int curRowOld = curRow - count - 1;
            QModelIndex newItemId = model()->index(curRow, 0, mainParent);
            QModelIndex oldItemId = model()->index(curRowOld, 0, mainParent);
            QVariant val = model()->data(oldItemId, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;

            least--;
        }
        model()->removeRows(startRow - count - 1, count, mainParent);
    }
}

bool TaskTreeView::checkAllAreChildren(const QModelIndex &parent, const QModelIndexList &selectedList) const
{
    foreach (const QModelIndex &id, selectedList)
    {
        QModelIndex currentParent = id.parent();
        bool foundParent(currentParent == parent);
        while (currentParent.isValid() && !foundParent)
        {
            currentParent = currentParent.parent();
            foundParent = currentParent == parent;
        }
        if (!foundParent)
            return false;
    }

    return true;
}

bool less(const QModelIndex &left, const QModelIndex &right)
{
    return left.row() < right.row();
}

void TaskTreeView::getItemChildren(const QModelIndex &parent,
                                   const QModelIndexList &selectedList,
                                   QModelIndexList &topLevelChildren) const
{
    topLevelChildren.clear();
    foreach (const QModelIndex &id, selectedList)
        if (id.parent().internalId() == parent.internalId())
            topLevelChildren.append(id);
    qSort(topLevelChildren.begin(), topLevelChildren.end(), less);
}

QModelIndex TaskTreeView::getFirstSelection(const QModelIndexList &selectedList) const
{
    if (!selectedList.count())
        return QModelIndex();

    QModelIndex curItem = model()->index(0, 0);
    while (curItem.isValid())
    {
        if (selectedList.contains(curItem))
            return curItem;
        if (curItem.child(0, 0).isValid())
        {
            curItem = curItem.child(0, 0);
        }
        else if (curItem.sibling(curItem.row() + 1, 0).isValid())
        {
            curItem = curItem.sibling(curItem.row() + 1, 0);
        }
        else
        {
            QModelIndex item = curItem.parent();
            while (item.isValid())
            {
                if (item.sibling(item.row() + 1, 0).isValid())
                {
                    curItem = item.sibling(item.row() + 1, 0);
                    if (selectedList.contains(curItem))
                        return curItem;
                    break;
                }
                item = item.parent();
            }
        }
    }
    return QModelIndex();
}

bool TaskTreeView::canMoveUp() const
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return false;

    QModelIndex firstItem = getFirstSelection(selectedList);

    QModelIndex mainParent = firstItem.parent();
    if (firstItem.row() < 1)
        return false;

    selectedList.removeOne(firstItem);
    if (selectedList.count() && !checkAllAreChildren(mainParent, selectedList))
        return false;
    return true;
}

bool TaskTreeView::canMoveDown() const
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return false;

    QModelIndex firstItem = getFirstSelection(selectedList);
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    int parentChildrenCount = model()->rowCount(mainParent);

    selectedList.removeOne(firstItem);
    if (selectedList.count())
    {
        if (!checkAllAreChildren(mainParent, selectedList))
            return false;

        QModelIndexList topLevelChildren;
        getItemChildren(mainParent, selectedList, topLevelChildren);
        itemsToMove.append(topLevelChildren);
    }

    const int count = itemsToMove.count();
    if (firstItem.row() + count >= parentChildrenCount)
        return false;

    return count > 0;
}

bool TaskTreeView::canMoveLeft() const
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return false;

    QModelIndex firstItem = getFirstSelection(selectedList);

    QModelIndex mainParent = firstItem.parent();
    if (!mainParent.isValid())
        return false;

    selectedList.removeOne(firstItem);
    if (selectedList.count() && !checkAllAreChildren(mainParent, selectedList))
        return false;
    return true;
}

bool TaskTreeView::canMoveRight() const
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return false;

    QModelIndex firstItem = getFirstSelection(selectedList);

    QModelIndex mainParent = firstItem.parent();
    if (firstItem.row() < 1)
        return false;

    selectedList.removeOne(firstItem);
    if (selectedList.count() && !checkAllAreChildren(mainParent, selectedList))
        return false;

    QModelIndex newParent = model()->index(firstItem.row() - 1, 0, mainParent);
    if (!newParent.isValid())
        return false;
    return true;
}
