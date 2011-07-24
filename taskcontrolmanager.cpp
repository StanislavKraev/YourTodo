#include <QLayout>
#include <QWidget>
#include <QPair>
#include <QItemSelectionModel>

#include "ipreferences.h"
#include "widgets/flowlayout.h"
#include "widgets/prioritywidget.h"
#include "widgets/spinnerwidget.h"
#include "widgets/datawidgetmodel.h"

#include "taskcontrolmanager.h"

TaskControlManager::TaskControlManager(QWidget *parentWidget, IPreferences *prefs) :
    m_parentWidget(parentWidget),
    m_prefs(prefs)
{
    m_parentWidget->setLayout(new FlowLayout(1, 1, 1));
}

void TaskControlManager::createTaskControls()
{
    if (m_parentWidget->layout()->count())
        return;

    typedef QPair<nsTaskData::TaskDataMember, DataWidget*> DataWidgetPair;
    QList<DataWidgetPair> pairs;
    pairs << DataWidgetPair(nsTaskData::Priority, new PriorityWidget("Priority", m_parentWidget))
          << DataWidgetPair(nsTaskData::PercentDone, new SpinnerWidget("% Complete", 0, 100, m_parentWidget)) ;

    foreach (const DataWidgetPair &pair, pairs)
    {
        m_widgets[pair.first] = pair.second;
        pair.second->setVisible(m_prefs->isColumnSelected(pair.first));
        m_parentWidget->layout()->addWidget(pair.second);
        pair.second->setModel(new DataWidgetModel(pair.first));
    }
}

TaskControlManager::~TaskControlManager()
{
}

void TaskControlManager::selectionChanged(QItemSelectionModel *selectionModel)
{
    QModelIndexList selectedRows = selectionModel->selectedRows();
    if (selectedRows.count() != 1)
    {
        foreach (DataWidget *widget, m_widgets)
            widget->model()->setTask(Task::Ptr(0));
    }
    else
    {
        QModelIndex id = selectedRows.first();
        m_curTask = id.data(Qt::UserRole).value<Task::Ptr>();
        foreach (DataWidget *widget, m_widgets)
        {
            DataWidgetModel *model = widget->model();
            model->setTask(m_curTask);
        }
    }
}
