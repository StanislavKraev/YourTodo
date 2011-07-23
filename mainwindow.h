#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenuBar;
class QStatusBar;
class QToolBar;

class UiManager;
class ITaskList;
class QItemSelectionModel;
class QAbstractItemModel;
class TaskTreeView;
class ITreeUiProvider;

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
    void updateTreeModel(ITaskList *taskList, ITreeUiProvider *treeUi);
    void maximizeTaskList(bool maximized);
    bool isTasklistMaximized() const;
    QItemSelectionModel *selectionModel();
    QAbstractItemModel *model();
    TaskTreeView *treeView() const;
    QWidget* controlsArea() const;
public slots:
    void onShortcut();
protected:
    virtual void changeEvent(QEvent *event);
signals:
    void onMainWindowMinimized();
    void onMainWindowRestored();
    void onModelsChanged(QItemSelectionModel *selectionModel, QAbstractItemModel *model);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
