#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;
class FileManager;
class UiManager;
class SelectionTool;
class TaskControlManager;

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();
public:
    MainWindow *mainWindow();
private:
    MainWindow *m_mainWindow;
    FileManager *m_fileManager;
    UiManager *m_uiManager;
    SelectionTool *m_selectionTool;
    TaskControlManager *m_taskControlManager;
};

#endif // APPLICATION_H
