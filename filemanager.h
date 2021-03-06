#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QList>
class QWidget;
class TaskList;
class ITaskList;

#include "tool.h"
#include "ifilemanager.h"

class FileManager : public QObject, public Tool, public IFileManager
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = 0);
    virtual ~FileManager();
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();
    const ITaskList *currentTaskList() const;
public:
    virtual void autoSave();
signals:
    void currentListChanged(ITaskList* newList);
    void filenameChanged(QString fileName);
public slots:
    void startUp();
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onClose();
private:
    bool loadTaskList(QString fileName);
private:
    QWidget *m_parent;
    QList<TaskList*> m_lists;
    TaskList *m_curTaskList;
};

#endif // FILEMANAGER_H
