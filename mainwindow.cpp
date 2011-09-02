#include "ui_mainwindow.h"

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTimer>
#include <QAbstractEventDispatcher>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>

#include <QxtGlobalShortcut>

#ifdef Q_WS_WIN
#include "windows.h"
#endif

#include "tasktree/tasklist.h"
#include "tasktree/treemodel.h"
#include "utils.h"
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

    ui->textEdit->setText("comment");

    restoreUiState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
 {
     if (true) {
         rememberUiState();
         emit(onMainWindowClosing());
         event->accept();
     } else {
         event->ignore();
     }
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

QTextEdit * MainWindow::commentsControl() const
{
    return ui->textEdit;
}

void MainWindow::rememberUiState()
{
    int windowWidth = width();
    int windowHeight = height();
    int left = pos().x();
    int top = pos().y();
    QList<int> splitter_sizes = ui->splitter->sizes();

    QSettings settings;
    settings.beginGroup("Ui");
    settings.setValue("width", windowWidth);
    settings.setValue("height", windowHeight);
    settings.setValue("left", left);
    settings.setValue("top", top);

    settings.beginWriteArray("splitter_size", splitter_sizes.length());
    for(int i = 0; i < splitter_sizes.length(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("size", splitter_sizes[i]);
    }
    settings.endArray();

    settings.endGroup();
}

void MainWindow::restoreUiState()
{
    QSettings settings;
    settings.beginGroup("Ui");
    int windowWidth = settings.value("width", 800).toInt();
    int windowHeight = settings.value("height", 600).toInt();
    int left = settings.value("left", 50).toInt();
    int top = settings.value("top", 50).toInt();

    QList<int> splitter_sizes;
    int splitter_size_count = settings.beginReadArray("splitter_size");
    for(int i = 0; i < splitter_size_count; ++i)
    {
        settings.setArrayIndex(i);
        int splitter_size = settings.value("size", 0).toInt();
        splitter_sizes.append(fixIntToRange(50, 1950, splitter_size));
    }
    settings.endArray();
    settings.endGroup();

    windowWidth = fixIntToRange(400, 2000, windowWidth);
    windowHeight = fixIntToRange(300, 1500, windowHeight);

    left = fixIntToRange(0, qApp->desktop()->width() - 50, left);
    top = fixIntToRange(0, qApp->desktop()->height() - 50, top);

    resize(windowWidth, windowHeight);
    move(left, top);
    ui->splitter->setSizes(splitter_sizes);
}

void MainWindow::setWindowTitle(const QString &title)
{
    QWidget::setWindowTitle(title + " - YourTodo");
}
