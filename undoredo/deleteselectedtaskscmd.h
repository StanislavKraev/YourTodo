#ifndef DELETESELECTEDTASKSCMD_H
#define DELETESELECTEDTASKSCMD_H

#include <QUndoCommand>

class DeleteSelectedTasksCmd : public QUndoCommand
{
public:
     DeleteSelectedTasksCmd();
     virtual void undo();
     virtual void redo();
};

#endif // DELETESELECTEDTASKSCMD_H
