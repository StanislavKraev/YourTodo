#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class QUndoStack;

class MainWindow;
class FileManager;
class UiManager;
class SelectionTool;
class TaskControlManager;
class PrefsManager;
class TaskEditorTool;

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();
public:
    MainWindow *mainWindow();
private:
    void createSettings();
private:
    MainWindow *m_mainWindow;
    FileManager *m_fileManager;
    UiManager *m_uiManager;
    SelectionTool *m_selectionTool;
    TaskControlManager *m_taskControlManager;
    PrefsManager *m_prefsManager;
    TaskEditorTool *m_taskEditTool;
    QUndoStack *m_undoStack;
};

#endif // APPLICATION_H
