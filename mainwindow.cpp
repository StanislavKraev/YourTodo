#include "ui_mainwindow.h"

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QDebug>

#include <QxtGlobalShortcut>

#ifdef Q_WS_WIN
#include "windows.h"
#endif

#include "tasktree/tasklist.h"
#include "tasktree/treemodel.h"
#include "application.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->setEditTriggers(QAbstractItemView::SelectedClicked |
                                  QAbstractItemView::EditKeyPressed);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->setAlternatingRowColors(true);
    QList<int> sizes;
    sizes << 300 << 100;
    ui->splitter->setSizes(sizes);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent * event)
{
    if(event->type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            emit(onMainWindowMinimized());
            qApp->processEvents();
            QTimer::singleShot(250, this, SLOT(hide()));
            return;
        }
        else
        {
            QWindowStateChangeEvent *ev = (QWindowStateChangeEvent*)event;
            if (ev->oldState() == Qt::WindowMinimized)
            {
                emit(onMainWindowRestored());
                qApp->processEvents();
                QTimer::singleShot(250, this, SLOT(show()));
                return;
            }
        }
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::SetupEventFilter()
{
    QxtGlobalShortcut* shortcut = new QxtGlobalShortcut(QKeySequence(Qt::CTRL|Qt::SHIFT|Qt::Key_T), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(onShortcut()));
}

void MainWindow::onShortcut()
{
    if (isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else
        showMinimized();
}

QMenuBar * MainWindow::menuBar()
{
    return ui->menuBar;
}

QStatusBar * MainWindow::statusBar()
{
    return ui->statusBar;
}

QToolBar * MainWindow::toolBar()
{
    return ui->mainToolBar;
}

void MainWindow::updateTreeModel(ITaskList *taskList, ITreeUiProvider *treeUi)
{
    QAbstractItemModel *oldModel = ui->treeView->model();

    TreeModel *model = new TreeModel(this, taskList, treeUi);
    ui->treeView->setModel(model);
    if (oldModel)
        delete oldModel;

    emit(onModelsChanged(ui->treeView->selectionModel(), model));
}

void MainWindow::maximizeTaskList(bool maximized)
{
    ui->textEdit->setVisible(!maximized);
    ui->controlsArea->setVisible(!maximized);
}

bool MainWindow::isTasklistMaximized() const
{
    bool visible = ui->textEdit->isVisible();
    return !visible;
}

QItemSelectionModel* MainWindow::selectionModel()
{
    return ui->treeView->selectionModel();
}

QAbstractItemModel * MainWindow::model()
{
    return ui->treeView->model();
}

TaskTreeView * MainWindow::treeView() const
{
    return ui->treeView;
}

QWidget * MainWindow::controlsArea() const
{
    return ui->controlsArea;
}
