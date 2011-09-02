#ifndef TASKTREEVIEW_H
#define TASKTREEVIEW_H

#include <QTreeView>
#include <QKeyEvent>
#include <QModelIndexList>

class TreeModel;

class TaskTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaskTreeView(QWidget *parent = 0);
public:
    bool canMoveUp() const;
    bool canMoveDown() const;
    bool canMoveLeft() const;
    bool canMoveRight() const;
    void shiftSelectedTasksRight();
    void shiftSelectedTasksLeft();
    void shiftSelectedTasksUp();
    void shiftSelectedTasksDown();
protected:
    virtual void keyPressEvent(QKeyEvent *event);
private:
    void toggleSelectedTasks();
    void addTaskBelowCursor();
    void removeSelectedTasks();
    void getSelectedRowsRange(const QModelIndexList &selectedList, int &startRow, int &endRow) const;
    bool checkAllAreChildren(const QModelIndex &parent, const QModelIndexList &selectedList) const;
    void getItemChildren(const QModelIndex &parent, const QModelIndexList &selectedList, QModelIndexList &topLevelChildren) const;
    QModelIndex getFirstSelection(const QModelIndexList &selectedList) const;
signals:
public slots:
private:
};

#endif // TASKTREEVIEW_H
