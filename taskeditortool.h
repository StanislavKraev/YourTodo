#ifndef TASKEDITORTOOL_H
#define TASKEDITORTOOL_H

#include <QObject>
#include "tool.h"

class IToolManager;
class QItemSelectionModel;
class TaskTreeView;

class TaskEditorTool : public QObject, public Tool
{
    Q_OBJECT
public:
    explicit TaskEditorTool(TaskTreeView *treeView);
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    virtual bool isActionEnabled(Actions::Actions action) const;
public slots:
    void selectionChanged(QItemSelectionModel *selectionModel);
    void moveUp();
    void moveDown();
    void moveRight();
    void moveLeft();
private:
    IToolManager *m_manager;
    QItemSelectionModel *m_curSelectionModel;
    TaskTreeView *m_taskTreeView;
};

#endif // TASKEDITORTOOL_H
