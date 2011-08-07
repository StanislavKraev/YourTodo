#ifndef TEXTEDITMODEL_H
#define TEXTEDITMODEL_H

#include <QObject>

#include "tasktree/itaskwatcher.h"
#include "tasktree/task.h"

class QTextEdit;

class TextEditModel : public QObject, public ITaskWatcher
{
    Q_OBJECT
public:
    explicit TextEditModel(QTextEdit *control);
public:
    void setTask(Task::Ptr task);
    virtual void taskChanged(nsTaskData::TaskDataMember member, Task* task);
signals:
private slots:
    void textChanged();
private:
    QTextEdit *m_control;
    Task::Ptr m_task;
};

#endif // TEXTEDITMODEL_H
