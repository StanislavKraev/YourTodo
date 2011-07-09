#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QStyledItemDelegate>
#include <QPainter>

#include "exceptions/loadtasksexception.h"

#include "tasktree/tasklist.h"
#include "tasktree/treemodel.h"
#include "tasktree/treeui.h"
#include "tasktree/xmltaskloader.h"

#include "uimanager.h"

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_treeUi(0)
{
    ui->setupUi(this);
#ifdef Q_WS_WIN
    XmlTaskLoader loader("c:\\temp\\list.tdl");
#else
    XmlTaskLoader loader("");
#endif
    TaskList *taskList = new TaskList();
    try
    {
        taskList->load(&loader);
    }
    catch (LoadTasksException &e)
    {
        QMessageBox messageBox;
        messageBox.warning(this, "Can't load task list", e.message());
        return;
    }

    QFont treeViewFont = QFont(ui->treeView->font());
    treeViewFont.setStrikeOut(true);

    m_treeUi = new TreeUi(treeViewFont);
    m_treeUi->addColumn(TreeColumnData("!", Priority));
    m_treeUi->addColumn(TreeColumnData("%", PercentDone));
    m_treeUi->addColumn(TreeColumnData("O", IconIndex));
    m_treeUi->addColumn(TreeColumnData("Title", Title));
    TreeModel *model = new TreeModel(this, taskList, m_treeUi);
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::SelectedClicked |
                                  QAbstractItemView::EditKeyPressed);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->header()->setResizeMode(0, QHeaderView::ResizeToContents);
    ui->treeView->header()->resizeSection(1, 30);
    ui->treeView->header()->resizeSection(2, 24);
    m_uiManager = new UiManager(this, ui->menuBar, ui->statusBar, ui->mainToolBar);
    m_uiManager->addTool(m_uiManager);
    m_uiManager->Init();
}

MainWindow::~MainWindow()
{
    if (m_uiManager)
        m_uiManager->deleteLater();
    m_uiManager = 0;

    if (m_treeUi)
        m_treeUi->deleteLater();
    delete ui;
}
