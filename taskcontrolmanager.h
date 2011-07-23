#ifndef TASKCONTROLMANAGER_H
#define TASKCONTROLMANAGER_H

#include <QObject>
#include "itaskcontrolmanager.h"

class ITreeUiProvider;
class QWidget;
class QItemSelectionModel;

class TaskControlManager : public QObject, public ITaskControlManager
{
    Q_OBJECT
public:
    TaskControlManager(QWidget *parentWidget);
    virtual ~TaskControlManager();
public:
    virtual void createTaskControls(ITreeUiProvider *treeUi);
public slots:
    void selectionChanged(QItemSelectionModel *selectionModel);
private:
    void clearAndDisableControls();
    void enableControls();
private:
    ITreeUiProvider *m_treeUi;
    QWidget *m_parentWidget;
};

#endif // TASKCONTROLMANAGER_H
