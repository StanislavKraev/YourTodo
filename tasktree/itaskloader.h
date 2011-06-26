#ifndef ITASKLOADER_H
#define ITASKLOADER_H

#include "tasktree/taskinfo.h"

class ITaskLoader
{
protected:
    ITaskLoader() {}
private:
    ITaskLoader(const ITaskLoader &);
    ITaskLoader& operator=(const ITaskLoader &);
public:
    virtual TaskInfo read() = 0;
};

#endif // ITASKLOADER_H