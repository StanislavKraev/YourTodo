#include "task.h"

Task::Task() : m_id(-1)
{
}

Task::Task(int id, QString title, unsigned short percentDone) :
    m_id(id),
    m_title(title),
    m_percentDone(percentDone)
{
}

Task::Task(const Task &task) :
    m_id(task.m_id),
    m_title(task.m_title),
    m_subTaskList(task.m_subTaskList),
    m_parent(task.m_parent),
    m_percentDone(task.m_percentDone)
{
}

QList<Task::Ptr>::Iterator Task::tasks()
{
    return m_subTaskList.begin();
}

QList<Task::Ptr>::ConstIterator Task::tasks() const
{
    return m_subTaskList.begin();
}

void Task::addTask(Ptr task)
{
    m_subTaskList.append(task);
}

int Task::id() const
{
    return m_id;
}

Task::Ptr Task::getAt(int index) const
{
    return m_subTaskList[index];
}

QString Task::title() const
{
    return m_title;
}

int Task::count() const
{
    return m_subTaskList.count();
}

int Task::pos(Task::Ptr subTask) const
{
    return m_subTaskList.indexOf(subTask);
}

Task::Ptr Task::parent() const
{
    return m_parent;
}

void Task::setParent(Task::Ptr parent)
{
    m_parent = parent;
}

void Task::setTitle(QString title)
{
    m_title = title;
}

unsigned short Task::percentDone() const
{
    return m_percentDone;
}

void Task::setPercentDone(unsigned short percentDone)
{
    m_percentDone = percentDone;
}

void Task::insertSubTask(int pos, Task::Ptr task)
{
    if (pos < 0 ||
        ((pos > count()) && (count() > 0)))
        return;
    m_subTaskList.insert(pos, task);
}

void Task::removeAt(int pos)
{
    if (pos < 0 || pos > count() - 1)
        return;
    m_subTaskList.removeAt(pos);
}

void Task::replace(Task::Ptr oldItem, Task::Ptr newItem)
{
    if (m_subTaskList.contains(oldItem))
    {
        int oldId = oldItem->m_id;
        oldItem->m_comments = newItem->m_comments;
        oldItem->m_commentsType = newItem->m_commentsType;
        oldItem->m_cost = newItem->m_cost;
        oldItem->m_creationDate = newItem->m_creationDate;
        oldItem->m_doneDate = newItem->m_doneDate;
        oldItem->m_iconIndex = newItem->m_iconIndex;
        oldItem->m_id = newItem->m_id;
        oldItem->m_lastMod = newItem->m_lastMod;
        oldItem->m_percentDone = newItem->m_percentDone;
        oldItem->m_pos = newItem->m_pos;
        oldItem->m_priority = newItem->m_priority;
        oldItem->m_priorityColor = newItem->m_priorityColor;
        oldItem->m_risk = newItem->m_risk;
        oldItem->m_startDate = newItem->m_startDate;
        oldItem->m_subTaskList = newItem->m_subTaskList;
        oldItem->m_textColor = newItem->m_textColor;
        oldItem->m_title = newItem->m_title;

        foreach (Task::Ptr subTask, oldItem->m_subTaskList)
            subTask->setParent(oldItem);

        newItem->m_id = oldId;
    }
}

