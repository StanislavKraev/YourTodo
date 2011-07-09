#ifndef ACTION_H
#define ACTION_H

#include "actionids.h"

class QAction;

class Action
{
public:
    Action();
    ~Action();
public:
    Actions::Actions id() const;
    bool enabled() const;
    void setEnabled(bool val);
private:
    QAction *m_action;
    Actions::Actions m_id;
};

#endif // ACTION_H
