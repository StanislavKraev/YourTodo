#ifndef XMLTASKSAVER_H
#define XMLTASKSAVER_H

#include "tasktree/itasksaver.h"

class XmlTaskSaver : public ITaskSaver
{
public:
    XmlTaskSaver();
public:
    virtual void save(TaskInfo info);
    virtual void init(QString fileName);
    virtual void finish();
};

#endif // XMLTASKSAVER_H
