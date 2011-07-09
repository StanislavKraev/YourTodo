#ifndef ITOOL_H
#define ITOOL_H

#include <QList>
#include <QObject>
#include "actionids.h"

class ITool
{
protected:
    ITool() {}
    ~ITool() {}
private:
    ITool(const ITool&);
    ITool& operator=(const ITool&);
public:
    virtual void getActions(QList<Actions::Actions> &actions) const = 0;
    virtual const char* getActionSlot(Actions::Actions action) const = 0;
    virtual QObject* getReciever() = 0;
    virtual bool isActionEnabled(Actions::Actions action) const = 0;
    virtual bool isActionChecked(Actions::Actions action) const = 0;
};

#endif // ITOOL_H
