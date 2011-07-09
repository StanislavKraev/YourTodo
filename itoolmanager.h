#ifndef ITOOLMANAGER_H
#define ITOOLMANAGER_H

#include "actionids.h"
class ITool;

class IToolManager
{
protected:
    IToolManager() {}
    ~IToolManager() {}
private:
    IToolManager(const IToolManager&);
    IToolManager& operator=(const IToolManager&);
public:
    virtual void addTool(ITool *tool) = 0;
    virtual void onActionChanged(Actions::Actions id) = 0;
};

#endif // ITOOLMANAGER_H
