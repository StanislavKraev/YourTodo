#ifndef ITASKCONTROLMANAGER_H
#define ITASKCONTROLMANAGER_H

class ITaskControlManager
{
protected:
    ITaskControlManager() {}
    ~ITaskControlManager() {}
private:
    ITaskControlManager& operator = (const ITaskControlManager&);
    ITaskControlManager(const ITaskControlManager&);
public:
    virtual void createTaskControls() = 0;
};

#endif // ITASKCONTROLMANAGER_H
