#include "tasktree/itaskwatcher.h"
#include "watchobserver.h"

void WatchObserver::addWatch(ITaskWatcher *watch)
{
    if (watch)
        m_watchers.insert(watch);
}

void WatchObserver::removeWatch(ITaskWatcher *watch)
{
    if (watch)
        m_watchers.remove(watch);
}

void WatchObserver::notifyMemberChange(nsTaskData::TaskDataMember member, Task *task)
{
    if (!task)
        return;

    foreach(ITaskWatcher *watcher, m_watchers)
        watcher->taskChanged(member, task);
}

