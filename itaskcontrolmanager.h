#ifndef ITASKCONTROLMANAGER_H
#define ITASKCONTROLMANAGER_H

class QTextEdit;

class ITaskControlManager
{
protected:
    ITaskControlManager() {}
    ~ITaskControlManager() {}
private:
    ITaskControlManager& operator = (const ITaskControlManager&);
    ITaskControlManager(const ITaskControlManager&);
public:
    virtual void createTaskControls(QTextEdit *commentsControl) = 0;
};

#endif // ITASKCONTROLMANAGER_H
