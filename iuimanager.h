#ifndef IUIMANAGER_H
#define IUIMANAGER_H

#include <QAction>
#include "actionids.h"

class IUiManager
{
protected:
    IUiManager() {}
    ~IUiManager() {}
private:
    IUiManager(const IUiManager&);
    IUiManager& operator=(const IUiManager&);
public:
    virtual const QAction* action(Actions::Actions action) const = 0;
    virtual void setActionShortcut(Actions::Actions action, QKeySequence sequence) = 0;
};

#endif // IUIMANAGER_H
