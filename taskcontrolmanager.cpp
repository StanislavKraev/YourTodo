#include <QLayout>
#include <QWidget>
#include <QItemSelectionModel>

#include "widgets/flowlayout.h"
#include "widgets/prioritywidget.h"
#include "widgets/spinnerwidget.h"

#include "tasktree/itreeuiprovider.h"
#include "tasktree/treecolumndata.h"

#include "taskcontrolmanager.h"

TaskControlManager::TaskControlManager(QWidget *parentWidget) :
    m_treeUi(0),
    m_parentWidget(parentWidget)
{
    m_parentWidget->setLayout(new FlowLayout(1, 1, 1));
}

void TaskControlManager::createTaskControls(ITreeUiProvider *treeUi)
{
    m_treeUi = treeUi;
    if (m_parentWidget->layout()->count())
        return;

    int count = m_treeUi->columnsCount();
    for (int i = 0; i < count; ++i)
    {
        TreeColumnData data = m_treeUi->columnData(i);
        QWidget *newWidget = 0;
        switch (data.taskDataMember)
        {
        case Priority:
            newWidget = new PriorityWidget("Priority", m_parentWidget);
            break;
        case PercentDone:
            newWidget = new SpinnerWidget("% Complete", 0, 100, m_parentWidget);
            break;
        }
        if (newWidget)
        {
            newWidget->setVisible(data.visible);
            m_parentWidget->layout()->addWidget(newWidget);
        }
    }
}

TaskControlManager::~TaskControlManager()
{
}

void TaskControlManager::selectionChanged(QItemSelectionModel *selectionModel)
{
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.count() != 1)
        clearAndDisableControls();
    else
    {
        enableControls();
    }
}

void TaskControlManager::clearAndDisableControls()
{

}

void TaskControlManager::enableControls()
{
}
