#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenuBar;
class QStatusBar;
class QToolBar;

class TreeUi;
class UiManager;
class ITaskList;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void SetupEventFilter();
    QMenuBar* menuBar();
    QStatusBar* statusBar();
    QToolBar* toolBar();
    void updateTreeModel(ITaskList *taskList);
    void maximizeTaskList(bool maximized);
    bool isTasklistMaximized() const;
public slots:
    void onShortcut();
protected:
    virtual void changeEvent(QEvent *event);
signals:
    void onMainWindowMinimized();
    void onMainWindowRestored();
private:
    Ui::MainWindow *ui;
    TreeUi *m_treeUi;
};

#endif // MAINWINDOW_H
