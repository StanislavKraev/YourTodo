#ifndef TOOL_H
#define TOOL_H

#include <QSet>
#include "actionids.h"
#include "itool.h"

class Tool : public ITool
{
public:
    virtual bool isActionEnabled(Actions::Actions action) const;
    virtual bool isActionChecked(Actions::Actions action) const;
    virtual void getActions(QList<Actions::Actions> &actions) const;
protected:
    void addAction(Actions::Actions action);
private:
    QSet<Actions::Actions> m_actionSet;
};

#endif // TOOL_H
