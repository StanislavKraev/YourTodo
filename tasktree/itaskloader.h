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
    virtual bool readHeader(QString &projectName, int &fileFormat,
                            int &uniqueId, int &fileVersion, QDateTime &earliestDueDate) = 0;
    virtual TaskInfo read() = 0;
    virtual QString fileName() const = 0;
};

#endif // ITASKLOADER_H
