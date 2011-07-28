#ifndef ITASKLIST_H
#define ITASKLIST_H

#include "task.h"

class ITaskLoader;
class ITaskSaver;
class ITaskWatcher;

class ITaskList
{
protected:
    ITaskList(){}
private:
    ITaskList(const ITaskList &);
    ITaskList &operator = (const ITaskList &);

public:
    virtual Task::ListIterator tasks() = 0;
    virtual Task::ListConstIterator tasks() const = 0;
    virtual Task::Ptr getAt(int index) const = 0;
    virtual Task::Ptr getById(int index) const = 0;
    virtual int pos(Task::Ptr task) const = 0;
    virtual int count() const = 0;
    virtual void add(Task::Ptr task) = 0;
    virtual Task::Ptr createTask(QString title) = 0;
    virtual bool insertNewTasks(Task::Ptr task, int pos, int count) = 0;
    virtual bool removeTasks(Task::Ptr task, int pos, int count) = 0;
    virtual Task::Ptr root() const = 0;
    virtual void replace(Task::Ptr oldItem, Task::Ptr newItem) = 0;
public:
    virtual void addWatch(ITaskWatcher* watch) = 0;
    virtual void removeWatch(ITaskWatcher* watch) = 0;
    virtual void notifyMemberChange(nsTaskData::TaskDataMember member, Task *task) = 0;
public:
    virtual bool load(ITaskLoader *loader) = 0;
    virtual bool save(ITaskSaver *saver) = 0;
};

#endif // ITASKLIST_H
