#ifndef ITASKSTORAGE_H
#define ITASKSTORAGE_H

#include "task.h"

class ITaskLoader;

class ITaskStorage
{
protected:
    ITaskStorage(){}
private:
    ITaskStorage(const ITaskStorage &);
    ITaskStorage &operator = (const ITaskStorage &);

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
public:
    virtual bool load(ITaskLoader *loader) = 0;
};

#endif // ITASKSTORAGE_H
