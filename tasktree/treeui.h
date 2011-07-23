#ifndef TREEUI_H
#define TREEUI_H

#include <QObject>
#include "tasktree/itreeuiprovider.h"

class QTreeView;

class TreeUi : public QObject, public ITreeUiProvider
{
public:
    TreeUi(const QFont &strikedOutFont, QTreeView *view);
public:
    void init();
public:
    virtual int columnsCount() const;
    virtual QString headerTitle(int section) const;
    virtual QVariant itemData(int column, Task::Ptr task) const;
    virtual void updateData(Task::Ptr task, int column, QVariant data);
    virtual QFont strikedOutFont() const;
    virtual QFont font(int column, Task::Ptr task) const;
    virtual QBrush foreground(int column, Task::Ptr task) const;
    virtual QBrush background(int column, Task::Ptr task) const;
    virtual TreeColumnData columnData(int column) const;
public:
    void addColumn(TreeColumnData columnData);
    void removeColumns();
private:
    QList<TreeColumnData> m_columns;
    QFont m_strikedOutFont;
    QTreeView *m_view;
};

#endif // TREEUI_H
