#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenuBar;
class QStatusBar;
class QToolBar;
class QTextEdit;

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
    QTextEdit* commentsControl() const;
public slots:
    void onShortcut();
    void setWindowTitle(const QString &title);
protected:
    virtual void changeEvent(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
private:
    void rememberUiState();
    void restoreUiState();
signals:
    void onMainWindowMinimized();
    void onMainWindowRestored();
    void onModelsChanged(QItemSelectionModel *selectionModel, QAbstractItemModel *model);
    void onMainWindowClosing();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
