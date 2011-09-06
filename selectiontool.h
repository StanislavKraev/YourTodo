#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QObject>
#include <QItemSelection>
#include "tool.h"

class IToolManager;
class QItemSelectionModel;
class QAbstractItemModel;
class TaskTreeView;

class SelectionTool : public QObject, public Tool
{
    Q_OBJECT
public:
    explicit SelectionTool(QItemSelectionModel* selectionModel, QAbstractItemModel* model, TaskTreeView* view);
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    virtual bool isActionEnabled(Actions::Actions action) const;
signals:
    void selectionChanged(QItemSelectionModel *selectionModel);
public slots:
    void onSelectAll();
    void onModelsChanged(QItemSelectionModel *selectionModel, QAbstractItemModel *model);
    void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
private:
    IToolManager *m_manager;
    QItemSelectionModel *m_selectionModel;
    QAbstractItemModel *m_model;
    TaskTreeView* m_view;
};

#endif // SELECTIONTOOL_H
