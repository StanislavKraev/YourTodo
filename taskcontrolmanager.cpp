#include <QLayout>
#include <QWidget>
#include <QItemSelectionModel>

#include "widgets/flowlayout.h"
#include "widgets/prioritywidget.h"
#include "widgets/spinnerwidget.h"

#include "taskcontrolmanager.h"

TaskControlManager::TaskControlManager(QWidget *parentWidget) :
    m_parentWidget(parentWidget)
{
    m_parentWidget->setLayout(new FlowLayout(1, 1, 1));
}

void TaskControlManager::createTaskControls()
{
    if (m_parentWidget->layout()->count())
        return;

    m_widgets[Priority] = new PriorityWidget("Priority", m_parentWidget);
    m_widgets[PercentDone] = new SpinnerWidget("% Complete", 0, 100, m_parentWidget);
    foreach (DataWidget *widget, m_widgets)
    {
        //widget->setVisible(data.visible);
        m_parentWidget->layout()->addWidget(widget);
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
