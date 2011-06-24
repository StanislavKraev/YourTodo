#ifndef TASKSTORAGE_H
#define TASKSTORAGE_H

#include <QMap>
#include "itaskstorage.h"

class TaskStorage : public ITaskStorage
{
public:
    TaskStorage();
public:
    virtual Task::ListIterator tasks();
    virtual Task::ListConstIterator tasks() const;
    virtual Task::Ptr getAt(int index) const;
    virtual Task::Ptr getById(int index) const;
    virtual int pos(Task::Ptr task) const;
    virtual int count() const;
    virtual void add(Task::Ptr task);
    virtual Task::Ptr createTask(QString title);
    virtual bool insertNewTasks(Task::Ptr task, int pos, int count);
    virtual bool removeTasks(Task::Ptr task, int pos, int count);
    virtual Task::Ptr root() const;
public:
    virtual bool load(ITaskLoader *loader);
protected:
    int nextId() const;
    void clear();
private:
    Task::Ptr m_taskRoot;
    QMap<int, Task::Ptr> m_idTaskMap;
};

#endif // TASKSTORAGE_H
