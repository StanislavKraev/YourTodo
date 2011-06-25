#include <QtXml/QDomDocument>

#include <QFile>
#include <QMap>

#include "exceptions/stopiterexception.h"
#include "itaskloader.h"

#include "taskstorage.h"

TaskStorage::TaskStorage()
{
    m_taskRoot = Task::Ptr(new Task());
}

Task::ListIterator TaskStorage::tasks()
{
    return m_taskRoot->tasks();
}

Task::ListConstIterator TaskStorage::tasks() const
{
    return m_taskRoot->tasks();
}

Task::Ptr TaskStorage::getAt(int index) const
{
    return m_taskRoot->getAt(index);
}

Task::Ptr TaskStorage::getById(int index) const
{
    return m_idTaskMap[index];
}

int TaskStorage::count() const
{
    return m_taskRoot->count();
}

void TaskStorage::add(Task::Ptr task)
{
    m_taskRoot->addTask(task);
}

int TaskStorage::nextId() const
{
    int maxKey = -1;
    foreach (int id, m_idTaskMap.keys())
    {
        if (maxKey < id)
            maxKey = id;
    }

    return maxKey + 1;
}

Task::Ptr TaskStorage::createTask(QString title)
{
    Task::Ptr newTask = Task::Ptr(new Task(nextId(), title, 0));
    add(newTask);
    m_idTaskMap[newTask->id()] = newTask;
    return newTask;
}

bool TaskStorage::load(ITaskLoader *loader)
{
    clear();

    Task::List allTasks;

    QMap<int, int> childParentMap;
    QMap<int, Task::Ptr> idTaskMap;

    bool stop = false;
    while (!stop)
    {
        try
        {
            TaskInfo info = loader->read();
            childParentMap.insert(info.id, info.parentId);
            Task::Ptr taskPtr = info.toTaskPtr();
            idTaskMap[info.id] = taskPtr;
            allTasks.append(taskPtr);
            m_idTaskMap[taskPtr->id()] = taskPtr;
        }
        catch (StopIterException)
        {
            stop = true;
        }
    }

    foreach (Task::Ptr task, allTasks)
    {
        int parentId = childParentMap[task->id()];
        if (parentId != -1)
        {
            Task::Ptr parent = idTaskMap[parentId];
            parent->addTask(task);
            task->setParent(parent);
        }
        else
            m_taskRoot->addTask(task);
    }

    return true;
}

void TaskStorage::clear()
{
    m_taskRoot = Task::Ptr(new Task());
    m_idTaskMap.clear();
}

int TaskStorage::pos(Task::Ptr task) const
{
    return m_taskRoot->pos(task);
}

bool TaskStorage::insertNewTasks(Task::Ptr task, int pos, int count)
{
    if (pos < 0 ||
        ((pos > task->count()) && (task->count() > 0)))
        return false;
    int curPos = pos;
    for (int taskToInsert = count; taskToInsert; taskToInsert--)
    {
        Task::Ptr newTask = Task::Ptr(new Task(nextId(), "", 0));
        newTask->setParent(task);
        m_idTaskMap[newTask->id()] = newTask;
        task->insertSubTask(curPos, newTask);
        ++curPos;
    }
    return true;
}

Task::Ptr TaskStorage::root() const
{
    return m_taskRoot;
}

bool TaskStorage::removeTasks(Task::Ptr task, int pos, int count)
{
    if (pos < 0 || (pos + count > task->count()))
        return false;

    for (int taskToRemove = count; taskToRemove; taskToRemove--)
    {
        Task::Ptr removingTask = task->getAt(pos);
        m_idTaskMap.remove(removingTask->id());
        task->removeAt(pos);
    }
    return true;
}
