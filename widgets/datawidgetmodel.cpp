#include "datawidgetmodel.h"

DataWidgetModel::DataWidgetModel(nsTaskData::TaskDataMember member) : QObject(),
    m_member(member)
{
}

void DataWidgetModel::taskChanged(nsTaskData::TaskDataMember member, Task* task)
{
    if (m_task && (member == m_member) && (task == m_task))
    {
        m_data = m_task->memberData(m_member);
        emit(dataChanged());
    }
}

QVariant DataWidgetModel::data() const
{
    return m_data;
}

void DataWidgetModel::setTask(Task::Ptr task)
{
    m_task = task;
    if (m_task)
        m_data = task->memberData(m_member);
    else
        m_data = QVariant();
    emit(dataChanged());
}

bool DataWidgetModel::enabled() const
{
    bool editable = m_task ? m_task->editable(m_member) : false;
    return m_data.isValid() && editable;
}

void DataWidgetModel::updateData(QVariant data)
{
    if (m_task && m_data.isValid())
        m_task->setMemberData(m_member, data);
}
