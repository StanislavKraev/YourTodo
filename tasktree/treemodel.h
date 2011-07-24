#ifndef TREEMODEL_H
#define TREEMODEL_H

#include "QAbstractItemModel"

#include "tasktree/itaskwatcher.h"

class ITaskList;
class ITreeUiProvider;

class TreeModel : public QAbstractItemModel, public ITaskWatcher
{
public:
    TreeModel(QObject* parent, ITaskList *taskStorage, ITreeUiProvider *treeUi);

public:
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex	index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex	parent(const QModelIndex &child) const;
    virtual int	rowCount(const QModelIndex &parent) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
public:
    virtual void taskChanged(nsTaskData::TaskDataMember member, Task* task);
private:
    QModelIndex idFromPtr(Task* task, QModelIndex parent = QModelIndex()) const;
private:
    ITaskList *m_taskList;
    ITreeUiProvider *m_treeUi;
};

#endif // TREEMODEL_H
