#include <QMessageBox>
#include <QFileDialog>

#include "tasktree/tasklist.h"
#include "tasktree/xmltaskloader.h"
#include "tasktree/xmltasksaver.h"
#include "exceptions/loadtasksexception.h"

#include "filemanager.h"

FileManager::FileManager(QWidget *parent) :
    QObject(parent),
    m_parent(parent),
    m_curTaskList(0)
{
}

FileManager::~FileManager()
{
    if (m_curTaskList)
        delete m_curTaskList;
}

void FileManager::onNew()
{
    if (m_curTaskList)
        delete m_curTaskList;
    m_curTaskList = new TaskList();
    emit(currentListChanged(m_curTaskList));
}

void FileManager::onOpen()
{
    QString fileName = QFileDialog::getOpenFileName(m_parent,
         tr("Open Tasklist"), "/home/kraevst/", tr("Task lists (*.tdl)"));
    if (fileName.isEmpty())
        return;

    XmlTaskLoader loader(fileName);
    TaskList *taskList = new TaskList();
    try
    {
        taskList->load(&loader);
    }
    catch (LoadTasksException &e)
    {
        QMessageBox messageBox;
        messageBox.warning(0, "Can't load task list", e.message());
        delete taskList;
        return;
    }
    //m_lists.append(taskList);
    TaskList* oldList = m_curTaskList;
    m_curTaskList = taskList;
    emit(currentListChanged(taskList));
    if (oldList)
        delete oldList;
}

void FileManager::onSave()
{
    if (m_curTaskList)
    {
        XmlTaskSaver saver;
        m_curTaskList->save(&saver);
    }
}

void FileManager::onSaveAs()
{
    if (m_curTaskList)
    {
        QString fileName = QFileDialog::getSaveFileName(m_parent,
             tr("Save Tasklist As"), "/home/kraevst/", tr("Task lists (*.tdl)"));
        if (fileName.isEmpty())
            return;
        m_curTaskList->setFileName(fileName);
        onSave();
    }
}

void FileManager::onClose()
{
}

void FileManager::init(IToolManager *manager)
{
    addAction(Actions::FileNew);
    addAction(Actions::FileOpen);
    addAction(Actions::FileSave);
    addAction(Actions::FileSaveAs);
}

const char * FileManager::getActionSlot(Actions::Actions action) const
{
    switch (action)
    {
    case Actions::FileNew:
        return SLOT(onNew());
    case Actions::FileOpen:
        return SLOT(onOpen());
    case Actions::FileSave:
        return SLOT(onSave());
    case Actions::FileSaveAs:
        return SLOT(onSaveAs());
    case Actions::FileClose:
        return SLOT(onClose());
    default:
        return 0;
    }
    return 0;
}

QObject *FileManager::getReciever()
{
    return this;
}
