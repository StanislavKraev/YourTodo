#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>

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
    QSettings settings;

    settings.beginGroup("List");

    bool setFile = false;
    if (m_curTaskList)
    {
        QString fileName = m_curTaskList->filePath();
        QFileInfo fi(fileName);
        if (fi.exists())
        {
            settings.setValue("current_path", fileName);
            setFile = true;
        }
    }
    if (!setFile)
        settings.remove("current_path");
    settings.endGroup();

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

    loadTaskList(fileName);
}

void FileManager::onSave()
{
    if (m_curTaskList)
    {
        if (m_curTaskList->filePath().length() < 1)
        {
            onSaveAs();
            return;
        }
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

        QFileInfo fi(fileName);
        if (fi.suffix().length() < 1)
            fileName += ".tdl";
        m_curTaskList->setFileName(fileName);
        onSave();
    }
}

void FileManager::onClose()
{
}

void FileManager::init(IToolManager *manager)
{
    (void)manager;
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

const ITaskList * FileManager::currentTaskList() const
{
    return m_curTaskList;
}

bool FileManager::loadTaskList(QString fileName)
{
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
        return false;
    }

    TaskList* oldList = m_curTaskList;
    m_curTaskList = taskList;
    emit(currentListChanged(taskList));
    if (oldList)
        delete oldList;
    return true;
}

void FileManager::startUp()
{
    QSettings settings;

    QString currentTaskListPath = settings.value("List/current_path", "").toString();

    QFileInfo fi(currentTaskListPath);
    if (!fi.exists())
        settings.remove("List/current_path");
    else
    {
        if (!loadTaskList(currentTaskListPath))
            settings.remove("List/current_path");
    }
    if (!m_curTaskList)
        onNew();
}
