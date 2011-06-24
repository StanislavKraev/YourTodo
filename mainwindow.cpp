#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include "exceptions/loadtasksexception.h"

#include "tasktree/taskstorage.h"
#include "tasktree/treemodel.h"
#include "tasktree/treeui.h"
#include "tasktree/xmltaskloader.h"

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
    TaskStorage *taskStorage = new TaskStorage();
    try
    {
        taskStorage->load(&loader);
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
    m_treeUi->addColumn(TreeColumnData("Title", Title));
    m_treeUi->addColumn(TreeColumnData("Comments", Comment));
    TreeModel *model = new TreeModel(this, taskStorage, m_treeUi);
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::SelectedClicked |
                                  QAbstractItemView::EditKeyPressed);
    ui->treeView->setTaskModel(model);
}

MainWindow::~MainWindow()
{
    if (m_treeUi)
        delete m_treeUi;
    delete ui;
}
