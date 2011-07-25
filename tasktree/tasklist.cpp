#include <QtXml/QDomDocument>

#include <QFile>
#include <QMap>

#include "exceptions/loadtasksexception.h"
#include "exceptions/stopiterexception.h"
#include "tasktree/itaskwatcher.h"
#include "itaskloader.h"
#include "itaskwatcher.h"
#include "itasksaver.h"

#include "tasklist.h"

TaskList::TaskList()
{
    m_taskRoot = Task::Ptr(new Task());
    m_lastModified = QDateTime::currentDateTime();
}

Task::ListIterator TaskList::tasks()
{
    return m_taskRoot->tasks();
}

Task::ListConstIterator TaskList::tasks() const
{
    return m_taskRoot->tasks();
}

Task::Ptr TaskList::getAt(int index) const
{
    return m_taskRoot->getAt(index);
}

Task::Ptr TaskList::getById(int index) const
{
    return m_idTaskMap[index];
}

int TaskList::count() const
{
    return m_taskRoot->count();
}

void TaskList::add(Task::Ptr task)
{
    m_taskRoot->addTask(task);
}

int TaskList::nextId() const
{
    int maxKey = -1;
    foreach (int id, m_idTaskMap.keys())
    {
        if (maxKey < id)
            maxKey = id;
    }

    return maxKey + 1;
}

Task::Ptr TaskList::createTask(QString title)
{
    Task::Ptr newTask = Task::Ptr(new Task(nextId(), title));
    add(newTask);
    m_idTaskMap[newTask->id()] = newTask;
    return newTask;
}

bool TaskList::load(ITaskLoader *loader)
{
    clear();
    m_fileName = loader->fileName();

    try
    {
        if (!loader->readHeader(m_projectName, m_fileFormat,
                                m_nextUniqueId, m_fileVersion, m_earliestDueDate))
            return false;
    }
    catch (LoadTasksException)
    {
        return false;
    }

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
        {
            m_taskRoot->addTask(task);
            task->setParent(m_taskRoot);
        }
    }

    return true;
}

void TaskList::clear()
{
    m_taskRoot = Task::Ptr(new Task());
    m_idTaskMap.clear();
}

int TaskList::pos(Task::Ptr task) const
{
    return m_taskRoot->pos(task);
}

bool TaskList::insertNewTasks(Task::Ptr task, int pos, int count)
{
    if (pos < 0 ||
        ((pos > task->count()) && (task->count() > 0)))
        return false;
    int curPos = pos;
    for (int taskToInsert = count; taskToInsert; taskToInsert--)
    {
        Task::Ptr newTask = Task::Ptr(new Task(nextId(), ""));
        newTask->setParent(task);
        m_idTaskMap[newTask->id()] = newTask;
        task->insertSubTask(curPos, newTask);
        ++curPos;
    }
    return true;
}

Task::Ptr TaskList::root() const
{
    return m_taskRoot;
}

bool TaskList::removeTasks(Task::Ptr task, int pos, int count)
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

void TaskList::replace(Task::Ptr oldItem, Task::Ptr newItem)
{
    Task::Ptr parent = oldItem->parent();
    m_idTaskMap[oldItem->id()] = newItem;
    m_idTaskMap[newItem->id()] = oldItem;
    parent->replace(oldItem, newItem);
}

bool TaskList::save(ITaskSaver *saver)
{
    saver->init(m_fileName);
    m_earliestDueDate = getEarliestDueDate();
    saver->saveHeader(m_projectName, m_fileFormat, nextId(),
                      m_fileVersion, m_earliestDueDate, m_lastModified);
    Task::Ptr curParent = m_taskRoot;
    int i = 0;
    int curCount = curParent->count();
    if (curCount < 1)
    {
        saver->finish();
        return false;
    }

    Task::Ptr curItem;
    while (true)
    {
        if (i >= curCount)
        {
            if (!curParent->parent())
                break;

            curItem = curParent;
            curParent = curParent->parent();
            if (!curParent)
                break;

            i = curParent->pos(curItem) + 1;
            saver->goUp();
            curCount = curParent->count();
        }
        else
        {
            curItem = curParent->getAt(i);
            saver->save(TaskInfo::fromTaskPtr(curItem));
            if (curItem->count())
            {
                curCount = curItem->count();
                i = 0;
                curParent = curItem;
                saver->goDown();
            }
            else if (i < curCount - 1)
            {
                i++;
            }
            else
            {
                curItem = curParent;
                curParent = curParent->parent();
                if (!curParent)
                    break;

                i = curParent->pos(curItem) + 1;
                saver->goUp();
                curCount = curParent->count();
            }
        }
    }
    saver->finish();
    return true;
}

QDateTime TaskList::getEarliestDueDate() const
{
    QDateTime date;
    foreach(Task::Ptr task, m_idTaskMap.values())
    {
        if (task->dueDate() < date)
            date = task->dueDate();
    }
    return date;
}

void TaskList::setFileName(QString fileName)
{
    m_fileName = fileName;
}

void TaskList::addWatch(ITaskWatcher *watch)
{
    if (watch)
        m_watchers.insert(watch);
}

void TaskList::removeWatch(ITaskWatcher *watch)
{
    if (watch)
        m_watchers.remove(watch);
}

void TaskList::notifyMemberChange(nsTaskData::TaskDataMember member, Task::Ptr task)
{
    if (!task)
        return;

    foreach(ITaskWatcher *watcher, m_watchers)
        watcher->taskChanged(member, task);
}

