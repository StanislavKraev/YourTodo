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
    // remove all items from the parent
    // add them to the end of the previous element (X)

    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    QModelIndex firstItem = selectedList.first();
    QModelIndexList itemsToMove;
    itemsToMove.append(firstItem);

    QModelIndex mainParent = firstItem.parent();
    if (firstItem.row() < 1)
        return;

    selectedList.pop_front();
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
        QModelIndex newParent = model()->index(firstItem.row() - 1, 0, mainParent);
        if (!newParent.isValid())
            return;

        int curRow = model()->rowCount(newParent);
        model()->insertRows(curRow, itemsToMove.count(), newParent);
        foreach (const QModelIndex id, itemsToMove)
        {
            QModelIndex newItemId = model()->index(curRow, 0, newParent);
            QVariant val = model()->data(id, Qt::UserRole);
            model()->setData(newItemId, val, Qt::UserRole);
            curRow++;
        }
        model()->removeRows(firstItem.row(), itemsToMove.count(), mainParent);
    }
}

void TaskTreeView::shiftSelectedTasksLeft()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

}

void TaskTreeView::shiftSelectedTasksUp()
{
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;
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
        bool foundParent(false);
        while (currentParent.isValid() && !foundParent)
        {
            foundParent = currentParent.internalId() == parent.internalId();
            currentParent = currentParent.parent();
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
