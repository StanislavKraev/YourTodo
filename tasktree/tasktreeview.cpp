#include "tasktree/treemodel.h"

#include "tasktreeview.h"

TaskTreeView::TaskTreeView(QWidget *parent) :
    QTreeView(parent),
    m_model(0)
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
    }
    else if (event->key() == Qt::Key_Delete &&
             event->modifiers() == Qt::NoModifier)
    {
        removeSelectedTasks();
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

void TaskTreeView::setTaskModel(TreeModel *model)
{
    m_model = model;
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
    // TODO: investigate how TODOlist removes multiple selection (part of hierarchy, several levels)
    // Now only items under 1 parent are removed correctly.
    QModelIndexList selectedList = selectionModel()->selectedRows(0);
    if (selectedList.count() < 1)
        return;

    int startRow = -1;
    int endRow = -1;

    getSelectedRowsRange(selectedList, startRow, endRow);

    QModelIndex parent = selectedList.first().parent();
    model()->removeRows(startRow, endRow - startRow + 1, parent);
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
