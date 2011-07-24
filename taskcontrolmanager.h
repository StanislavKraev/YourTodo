#ifndef TASKCONTROLMANAGER_H
#define TASKCONTROLMANAGER_H

#include <QObject>
#include <QList>
#include "widgets/datawidget.h"
#include "itaskcontrolmanager.h"
#include "tasktree/treecolumndata.h"

class QWidget;
class QItemSelectionModel;

class TaskControlManager : public QObject, public ITaskControlManager
{
    Q_OBJECT
public:
    TaskControlManager(QWidget *parentWidget);
    virtual ~TaskControlManager();
public:
    virtual void createTaskControls();
public slots:
    void selectionChanged(QItemSelectionModel *selectionModel);
private:
    void clearAndDisableControls();
    void enableControls();
private:
    QWidget *m_parentWidget;
    QMap<TaskDataMember, DataWidget*> m_widgets;
};

#endif // TASKCONTROLMANAGER_H
