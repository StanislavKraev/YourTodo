#ifndef ITASKSAVER_H
#define ITASKSAVER_H

#include "tasktree/taskinfo.h"

class ITaskSaver
{
protected:
    ITaskSaver() {}
    ~ITaskSaver() {}
private:
    ITaskSaver(const ITaskSaver&);
    ITaskSaver& operator = (const ITaskSaver&);
public:
    virtual void init(QString fileName) = 0;
    virtual void saveHeader(QString projectName, int fileFormat,
                            int uniqueId, int fileVersion, QDateTime earliestDueDate,
                            QDateTime lastModified) = 0;
    virtual void save(TaskInfo info) = 0;
    virtual void finish() = 0;
    virtual void goDown() = 0;
    virtual void goUp() = 0;
};

#endif // ITASKSAVER_H
