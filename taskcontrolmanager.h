#ifndef TASKCONTROLMANAGER_H
#define TASKCONTROLMANAGER_H

#include <QObject>
#include <QList>
#include "widgets/datawidget.h"
#include "itaskcontrolmanager.h"
#include "tasktree/treecolumndata.h"
#include "tasktree/task.h"

class QWidget;
class QItemSelectionModel;
class IPreferences;
class ITaskList;
class TextEditModel;

class TaskControlManager : public QObject, public ITaskControlManager
{
    Q_OBJECT
public:
    TaskControlManager(QWidget *parentWidget, IPreferences *prefs);
    virtual ~TaskControlManager();
public:
    virtual void createTaskControls(QTextEdit *commentsControl);
public slots:
    void selectionChanged(QItemSelectionModel *selectionModel);
    void onCurrentListChanged(ITaskList* newList);
private:
    QWidget *m_parentWidget;
    QMap<nsTaskData::TaskDataMember, DataWidget*> m_widgets;
    IPreferences *m_prefs;
    Task::Ptr m_curTask;
    ITaskList* m_taskList;
    TextEditModel* m_commentsModel;
};

#endif // TASKCONTROLMANAGER_H
