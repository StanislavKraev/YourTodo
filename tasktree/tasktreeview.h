#ifndef TASKTREEVIEW_H
#define TASKTREEVIEW_H

#include <QTreeView>
#include <QKeyEvent>

class TreeModel;

class TaskTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaskTreeView(QWidget *parent = 0);
public:
    void setTaskModel(TreeModel *model);
protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void toggleSelectedTasks();
    void addTaskBelowCursor();
    void removeSelectedTasks();
    void getSelectedRowsRange(const QModelIndexList &selectedList, int &startRow, int &endRow) const;
signals:

public slots:
private:
    TreeModel *m_model;
};

#endif // TASKTREEVIEW_H
