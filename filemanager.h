#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QList>
class QWidget;
class TaskList;
class ITaskList;

#include "tool.h"

class FileManager : public QObject, public Tool
{
    Q_OBJECT
public:
    explicit FileManager(QWidget *parent = 0);
public:
    virtual void init(IToolManager *manager);
    virtual const char* getActionSlot(Actions::Actions action) const;
    virtual QObject* getReciever();

signals:
    void currentListChanged(ITaskList* newList);
public slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onClose();
private:
    QWidget *m_parent;
    QList<TaskList*> m_lists;
};

#endif // FILEMANAGER_H
