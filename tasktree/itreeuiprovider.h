#ifndef ITREEUIPROVIDER_H
#define ITREEUIPROVIDER_H

#include <QFont>
#include <QString>
#include <QVariant>

#include "tasktree/task.h"

class ITreeUiProvider
{
protected:
    ITreeUiProvider() {}
private:
    ITreeUiProvider(const ITreeUiProvider&);
    ITreeUiProvider& operator=(const ITreeUiProvider&);
public:
    virtual int columnsCount() const = 0;
    virtual QString headerTitle(int section) const = 0;
    virtual QVariant itemData(int column, Task::Ptr task) = 0;
    virtual void updateData(Task::Ptr task, int column, QVariant data) = 0;
    virtual QFont strikedOutFont() const = 0;
};


#endif // ITREEUIPROVIDER_H
