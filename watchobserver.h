#ifndef WATCHOBSERVER_H
#define WATCHOBSERVER_H

#include <QSet>
#include "QSharedPointer"

#include "tasktree/treecolumndata.h"

class ITaskWatcher;
class Task;

class WatchObserver
{
public:
    void addWatch(ITaskWatcher* watch);
    void removeWatch(ITaskWatcher* watch);
    void notifyMemberChange(nsTaskData::TaskDataMember member, Task *task);
private:
    QSet<ITaskWatcher*> m_watchers;
};

#endif // WATCHOBSERVER_H
