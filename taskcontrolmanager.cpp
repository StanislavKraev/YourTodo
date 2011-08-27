#include <QLayout>
#include <QWidget>
#include <QPair>
#include <QItemSelectionModel>
#include <QTextEdit>

#include "ipreferences.h"
#include "tasktree/itasklist.h"
#include "widgets/flowlayout.h"
#include "widgets/prioritywidget.h"
#include "widgets/spinnerwidget.h"
#include "widgets/datawidgetmodel.h"
#include "widgets/texteditmodel.h"
#include "widgets/floatpointingwidget.h"
#include "widgets/comboboxwidget.h"

#include "taskcontrolmanager.h"

TaskControlManager::TaskControlManager(QWidget *parentWidget, IPreferences *prefs) :
    m_parentWidget(parentWidget),
    m_prefs(prefs),
    m_taskList(0),
    m_commentsModel(0)
{
    m_parentWidget->setLayout(new FlowLayout(1, 1, 1));
}

void TaskControlManager::createTaskControls(QTextEdit *commentsControl)
{
    m_commentsModel = new TextEditModel(commentsControl);

    if (m_parentWidget->layout()->count())
        return;

    typedef QPair<nsTaskData::TaskDataMember, DataWidget*> DataWidgetPair;
    QList<DataWidgetPair> pairs;
    pairs << DataWidgetPair(nsTaskData::Priority, new PriorityWidget("Priority", m_parentWidget))
          << DataWidgetPair(nsTaskData::Risk, new ComboBoxWidget("Risk", m_parentWidget))
          << DataWidgetPair(nsTaskData::PercentDone, new SpinnerWidget("% Complete", 0, 100, m_parentWidget))
          << DataWidgetPair(nsTaskData::Cost, new FloatPointingWidget("Cost", m_parentWidget));

    foreach (const DataWidgetPair &pair, pairs)
    {
        m_widgets[pair.first] = pair.second;
        pair.second->setVisible(m_prefs->isColumnSelected(pair.first));
        m_parentWidget->layout()->addWidget(pair.second);
        DataWidgetModel *model = new DataWidgetModel(pair.first);
        pair.second->setModel(model);
        if (m_taskList)
            m_taskList->addWatch(model);
    }
    if (m_taskList)
        m_taskList->addWatch(m_commentsModel);
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
        m_commentsModel->setTask(Task::Ptr(0));
    }
    else
    {
        QModelIndex id = selectedRows.first();
        QVariant var = id.data(Qt::UserRole);

        m_curTask = var.value<Task::Ptr>();
        m_commentsModel->setTask(m_curTask);
        foreach (DataWidget *widget, m_widgets)
        {
            DataWidgetModel *model = widget->model();
            model->setTask(m_curTask);
        }
    }
}

void TaskControlManager::onCurrentListChanged(ITaskList *newList)
{
    if (m_taskList)
    {
        m_taskList->removeWatch(m_commentsModel);
        foreach(DataWidget* widget, m_widgets.values())
            m_taskList->removeWatch(widget->model());
    }
    m_taskList = newList;
    if (m_taskList)
    {
        m_taskList->addWatch(m_commentsModel);
        foreach(DataWidget* widget, m_widgets.values())
            m_taskList->addWatch(widget->model());
    }
}
