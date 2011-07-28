#ifndef DATAWIDGETMODEL_H
#define DATAWIDGETMODEL_H

#include <QObject>

#include "tasktree/itaskwatcher.h"
#include "tasktree/task.h"
#include "tasktree/treecolumndata.h"

class DataWidgetModel : public QObject, public ITaskWatcher
{
    Q_OBJECT
public:
    explicit DataWidgetModel(nsTaskData::TaskDataMember member);
public:
    QVariant data() const;
    bool enabled() const;
    void setTask(Task::Ptr task);
    void updateData(QVariant data);
public:
    virtual void taskChanged(nsTaskData::TaskDataMember member, Task* task);
signals:
    void dataChanged();
public slots:
private:
    QVariant m_data;
    Task::Ptr m_task;
    nsTaskData::TaskDataMember m_member;
};

#endif // DATAWIDGETMODEL_H
