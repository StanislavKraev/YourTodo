#ifndef TASKLIST_H
#define TASKLIST_H

#include <QMap>
#include <QString>
#include <QDateTime>
#include <QSet>
#include "itaskwatcher.h"
#include "itasklist.h"

class TaskList : public ITaskList
{
public:
    TaskList();
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
    virtual void replace(Task::Ptr oldItem, Task::Ptr newItem);
    void setFileName(QString fileName);
public:
    virtual void addWatch(ITaskWatcher* watch);
    virtual void removeWatch(ITaskWatcher* watch);
    virtual void notifyMemberChange(nsTaskData::TaskDataMember member, Task* task);
public:
    virtual bool load(ITaskLoader *loader);
    virtual bool save(ITaskSaver *saver);
    virtual QString filePath() const;
    virtual QString title() const;
protected:
    int nextId() const;
    void clear();
    QDateTime getEarliestDueDate() const;
private:
    QSet<ITaskWatcher*> m_watchers;
    Task::Ptr m_taskRoot;
    QMap<int, Task::Ptr> m_idTaskMap;
    QString m_fileName;
    QString m_projectName;
    int m_fileFormat;
    int m_nextUniqueId;
    int m_fileVersion;
    QDateTime m_earliestDueDate;
    QDateTime m_lastModified;
};

#endif // TASKLIST_H
