#ifndef TREEUI_H
#define TREEUI_H

#include "tasktree/itreeuiprovider.h"
#include "tasktree/treecolumndata.h"

class TreeUi : public ITreeUiProvider
{
public:
    TreeUi(const QFont &strikedOutFont);
public:
    virtual int columnsCount() const;
    virtual QString headerTitle(int section) const;
    virtual QVariant itemData(int column, Task::Ptr task);
    virtual void updateData(Task::Ptr task, int column, QVariant data);
    virtual QFont strikedOutFont() const;
public:
    void addColumn(TreeColumnData columnData);
    void removeColumns();
private:
    QList<TreeColumnData> m_columns;
    QFont m_strikedOutFont;
};

#endif // TREEUI_H
