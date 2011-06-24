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

