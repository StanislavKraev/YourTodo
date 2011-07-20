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
#include "tasktree/treeui.h"
#include "application.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_treeUi(0)
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
    if (m_treeUi)
        m_treeUi->deleteLater();
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

void MainWindow::updateTreeModel(ITaskList *taskList)
{
    if (m_treeUi)
        delete m_treeUi;
    QAbstractItemModel *oldModel = ui->treeView->model();

    QFont treeViewFont = QFont(ui->treeView->font());
    treeViewFont.setStrikeOut(true);
    m_treeUi = new TreeUi(treeViewFont, ui->treeView);
    m_treeUi->addColumn(TreeColumnData("Title", Title, -1));
    m_treeUi->addColumn(TreeColumnData("!", Priority, 22, TreeColumnData::PRIORITY));
    m_treeUi->addColumn(TreeColumnData("%", PercentDone, 30));
    m_treeUi->addColumn(TreeColumnData("O", IconIndex, 20, TreeColumnData::ICONINDEX));
    m_treeUi->addColumn(TreeColumnData("Pos", Position, 24));
    m_treeUi->addColumn(TreeColumnData("Risk", Risk, 30));
    m_treeUi->addColumn(TreeColumnData("Cost", Cost, 30));
    m_treeUi->addColumn(TreeColumnData("Start date", StartDate, 80));
    m_treeUi->addColumn(TreeColumnData("Done date", DoneDate, 80));
    m_treeUi->addColumn(TreeColumnData("Creation date", CreationDate, 80));
    m_treeUi->addColumn(TreeColumnData("Last mod", LastModified, 80));
    m_treeUi->addColumn(TreeColumnData("CT", CommentsType, 24));
    m_treeUi->addColumn(TreeColumnData("Comments", Comments));

    TreeModel *model = new TreeModel(this, taskList, m_treeUi);
    ui->treeView->setModel(model);
    if (oldModel)
        delete oldModel;

    m_treeUi->init();
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
