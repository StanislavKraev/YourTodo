#include <QItemSelectionModel>
#include <QUndoStack>

#include "itoolmanager.h"
#include "undoredo/deleteselectedtaskscmd.h"
#include "tasktree/tasktreeview.h"

#include "taskeditortool.h"

TaskEditorTool::TaskEditorTool(TaskTreeView *treeView, QUndoStack *undoStack) :
    m_manager(0), m_curSelectionModel(0), m_taskTreeView(treeView), m_undoStack(undoStack)
{
}

void TaskEditorTool::init(IToolManager *manager)
{
    m_manager = manager;
    addAction(Actions::MoveUp);
    addAction(Actions::MoveDown);
    addAction(Actions::MoveRight);
    addAction(Actions::MoveLeft);

    addAction(Actions::EditDeleteSelected);
    addAction(Actions::NewTaskBelow);
    addAction(Actions::NewTaskAbove);
    addAction(Actions::NewSubtask);
    addAction(Actions::MarkAsCompleted);
}

const char *TaskEditorTool::getActionSlot(Actions::Actions action) const
{
    if (action == Actions::MoveUp)
        return SLOT(moveUp());
    else if (action == Actions::MoveDown)
        return SLOT(moveDown());
    else if (action == Actions::MoveLeft)
        return SLOT(moveLeft());
    else if (action == Actions::MoveRight)
        return SLOT(moveRight());
    else if (action == Actions::EditDeleteSelected)
        return SLOT(editDeleteSelected());
    else if (action == Actions::NewTaskBelow)
        return SLOT(newTaskBelow());
    else if (action == Actions::NewTaskAbove)
        return SLOT(newTaskAbove());
    else if (action == Actions::NewSubtask)
        return SLOT(newSubTask());
    else if (action == Actions::MarkAsCompleted)
        return SLOT(markAsCompleted());
    return 0;
}

QObject *TaskEditorTool::getReciever()
{
    return this;
}

bool TaskEditorTool::isActionEnabled(Actions::Actions action) const
{
    if (!m_curSelectionModel)
        return false;

    if (action == Actions::NewTaskBelow)
        return m_taskTreeView->canAddBelow();
    else if (action == Actions::NewTaskAbove)
        return m_taskTreeView->canAddAbove();
    else if (action == Actions::NewSubtask)
        return m_taskTreeView->canAddSubtask();

    if (m_curSelectionModel->selectedRows().count() < 1)
        return false;

    if (action == Actions::MoveUp)
        return m_taskTreeView->canMoveUp();
    else if (action == Actions::MoveDown)
        return m_taskTreeView->canMoveDown();
    else if (action == Actions::MoveLeft)
        return m_taskTreeView->canMoveLeft();
    else if (action == Actions::MoveRight)
        return m_taskTreeView->canMoveRight();
    else if (action == Actions::EditDeleteSelected ||
             action == Actions::MarkAsCompleted)
        return true;
    return false;
}

void TaskEditorTool::selectionChanged(QItemSelectionModel *selectionModel)
{
    m_curSelectionModel = selectionModel;
    m_manager->onActionChanged(Actions::MoveDown);
    m_manager->onActionChanged(Actions::MoveUp);
    m_manager->onActionChanged(Actions::MoveLeft);
    m_manager->onActionChanged(Actions::MoveRight);
    m_manager->onActionChanged(Actions::EditDeleteSelected);
    m_manager->onActionChanged(Actions::NewTaskBelow);
    m_manager->onActionChanged(Actions::NewTaskAbove);
    m_manager->onActionChanged(Actions::NewSubtask);
    m_manager->onActionChanged(Actions::MarkAsCompleted);
}

void TaskEditorTool::moveUp()
{
    m_taskTreeView->shiftSelectedTasksUp();
}

void TaskEditorTool::moveDown()
{
    m_taskTreeView->shiftSelectedTasksDown();
}

void TaskEditorTool::moveRight()
{
    m_taskTreeView->shiftSelectedTasksRight();
}

void TaskEditorTool::moveLeft()
{
    m_taskTreeView->shiftSelectedTasksLeft();
}

void TaskEditorTool::editDeleteSelected()
{
    m_taskTreeView->removeSelectedTasks();
    m_undoStack->push(new DeleteSelectedTasksCmd());
}

void TaskEditorTool::newTaskBelow()
{
    m_taskTreeView->addTaskBelowCursor();
}

void TaskEditorTool::newTaskAbove()
{
    m_taskTreeView->addTaskAboveCursor();
}

void TaskEditorTool::newSubTask()
{
    m_taskTreeView->addSubTask();
}

void TaskEditorTool::markAsCompleted()
{
    m_taskTreeView->toggleSelectedTasks();
}
