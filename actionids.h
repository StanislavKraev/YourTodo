#ifndef ACTIONIDS_H
#define ACTIONIDS_H

namespace Actions
{
    enum Actions
    {
        Undefined = 0,
        FileNew,
        FileOpen,
        FileSave,
        FileSaveAs,
        FileSaveAll,
        FileClose,
        FileCloseAll,
        FileMinimize,
        FileExit,

        NewTaskAbove,
        NewTaskBelow,
        NewSubtask,

        EditUndo,
        EditRedo,
        EditCut,
        EditCopy,
        EditPaste,
        EditDeleteSelected,
        EditDeleteAll,
        EditSelectAll,
        EditPreferences,

        ViewMaxTasklist,
        ViewMaxComments,
        ViewExpandAll,
        ViewCollapseAll,
        ViewSelectColumns,
        ViewShowToolbar,
        ViewShowStatusbar,

        MoveUp,
        MoveDown,
        MoveLeft,
        MoveRight
    };
}
#endif // ACTIONIDS_H
