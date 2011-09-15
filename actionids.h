#ifndef ACTIONIDS_H
#define ACTIONIDS_H

#include <QString>

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
        MoveRight,
        MarkAsCompleted
        // DO NOT FORGET TO ADD NEW ACTION NAME BELOW
    };

    static QString actionNames[] = {
        "Undefined",
        "FileNew",
        "FileOpen",
        "FileSave",
        "FileSaveAs",
        "FileSaveAll",
        "FileClose",
        "FileCloseAll",
        "FileMinimize",
        "FileExit",

        "NewTaskAbove",
        "NewTaskBelow",
        "NewSubtask",

        "EditUndo",
        "EditRedo",
        "EditCut",
        "EditCopy",
        "EditPaste",
        "EditDeleteSelected",
        "EditDeleteAll",
        "EditSelectAll",
        "EditPreferences",

        "ViewMaxTasklist",
        "ViewMaxComments",
        "ViewExpandAll",
        "ViewCollapseAll",
        "ViewSelectColumns",
        "ViewShowToolbar",
        "ViewShowStatusbar",

        "MoveUp",
        "MoveDown",
        "MoveLeft",
        "MoveRight",
        "MarkAsCompleted"
    };
}
#endif // ACTIONIDS_H
