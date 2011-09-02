#include <QItemSelectionModel>

#include "itoolmanager.h"
#include "tasktree/tasktreeview.h"

#include "taskeditortool.h"

TaskEditorTool::TaskEditorTool(TaskTreeView *treeView) : m_manager(0), m_curSelectionModel(0), m_taskTreeView(treeView)
{
}

void TaskEditorTool::init(IToolManager *manager)
{
    m_manager = manager;
    addAction(Actions::MoveUp);
    addAction(Actions::MoveDown);
    addAction(Actions::MoveRight);
    addAction(Actions::MoveLeft);
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
    return 0;
}

QObject *TaskEditorTool::getReciever()
{
    return this;
}

bool TaskEditorTool::isActionEnabled(Actions::Actions action) const
{
    if (!m_curSelectionModel || m_curSelectionModel->selectedRows().count() < 1)
        return false;

    if (action == Actions::MoveUp)
        return m_taskTreeView->canMoveUp();
    else if (action == Actions::MoveDown)
        return m_taskTreeView->canMoveDown();
    else if (action == Actions::MoveLeft)
        return m_taskTreeView->canMoveLeft();
    else if (action == Actions::MoveRight)
        return m_taskTreeView->canMoveRight();
    return false;
}

void TaskEditorTool::selectionChanged(QItemSelectionModel *selectionModel)
{
    m_curSelectionModel = selectionModel;
    m_manager->onActionChanged(Actions::MoveDown);
    m_manager->onActionChanged(Actions::MoveUp);
    m_manager->onActionChanged(Actions::MoveLeft);
    m_manager->onActionChanged(Actions::MoveRight);
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
