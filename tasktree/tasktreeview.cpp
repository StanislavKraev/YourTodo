#include <QMap>

#include "tasktree/task.h"

#include "tasktreeview.h"

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
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() != 1)
        return;

    QModelIndex selection = selectedList.first();
    int row = selection.row() + 1;
    QModelIndex parent = selection.parent();
    model()->insertRows(row, 1, parent);
    QModelIndex newItemIndex = model()->index(row, 0, selection.parent());
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
    // + get selection
    // + find parent of the first element
    // + check that all following selected items are children of this parent. if not - cancel
    // + check that previous (prior to the first selected - X) element is the same level as the first
    // + find selected items of the same level as the first element
    // + remove all items from the parent
    // + add them to the end of the previous element (X)

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
        // TODO: store selection, expanded/closed items
        QModelIndex newParent = model()->index(firstItem.row() - 1, 0, mainParent);
        if (!newParent.isValid())
            return;

        int startRow = model()->rowCount(newParent);
        int curRow = model()->rowCount(newParent);
        model()->insertRows(startRow, itemsToMove.count(), newParent);
        // TODO: do not use model indexes and row numbers - they are shifted.
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, newParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
        expand(model()->index(newParent.row(), 0, mainParent));

        // TODO: apply stored selection & expanded/closed items.
        selectionModel()->select(QItemSelection(model()->index(startRow, 0, newParent),
                                                model()->index(startRow + itemsToMove.count() - 1, 0, newParent)),
                                 QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
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
        // TODO: store selection, expanded/closed items
        QModelIndex newParent = mainParent.parent();
        int startRow = mainParent.row() + 1;
        int curRow = mainParent.row() + 1;
        model()->insertRows(startRow, itemsToMove.count(), newParent);
        // TODO: do not use model indexes and row numbers - they are shifted.
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, newParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
        // TODO: apply stored selection & expanded/closed items.
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
        int startRow = firstItem.row() - 1;
        int curRow = startRow;
        model()->insertRows(startRow, itemsToMove.count(), mainParent);
        // TODO: do not use model indexes and row numbers - they are shifted.
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, mainParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        //model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
    }
}

void TaskTreeView::shiftSelectedTasksDown()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;
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

void TaskTreeView::getItemChildren(const QModelIndex &parent,
                                   const QModelIndexList &selectedList,
                                   QModelIndexList &topLevelChildren) const
{
    topLevelChildren.clear();
    foreach (const QModelIndex &id, selectedList)
        if (id.parent().internalId() == parent.internalId())
            topLevelChildren.append(id);
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
