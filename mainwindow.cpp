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

    QFont treeViewFont = QFont(ui->treeView->font());
    treeViewFont.setStrikeOut(true);

    m_treeUi = new TreeUi(treeViewFont, ui->treeView);
    m_treeUi->addColumn(TreeColumnData("!", Priority, TreeColumnData::PRIORITY));
    m_treeUi->addColumn(TreeColumnData("%", PercentDone));
    m_treeUi->addColumn(TreeColumnData("O", IconIndex));
    m_treeUi->addColumn(TreeColumnData("Title", Title));

    ui->treeView->setEditTriggers(QAbstractItemView::SelectedClicked |
                                  QAbstractItemView::EditKeyPressed);
    ui->treeView->setUniformRowHeights(true);
//    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
//    ui->treeView->header()->resizeSection(1, 30);
//    ui->treeView->header()->resizeSection(2, 24);
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
    QAbstractItemModel *oldModel = ui->treeView->model();

    TreeModel *model = new TreeModel(this, taskList, m_treeUi);
    ui->treeView->setModel(model);
    if (oldModel)
        delete oldModel;
}
