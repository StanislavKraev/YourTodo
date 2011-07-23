#ifndef ITASKCONTROLMANAGER_H
#define ITASKCONTROLMANAGER_H

class ITreeUiProvider;

class ITaskControlManager
{
protected:
    ITaskControlManager() {}
    ~ITaskControlManager() {}
private:
    ITaskControlManager& operator = (const ITaskControlManager&);
    ITaskControlManager(const ITaskControlManager&);
public:
    virtual void createTaskControls(ITreeUiProvider *treeUi) = 0;
};

#endif // ITASKCONTROLMANAGER_H
