#include "tool.h"

void Tool::addAction(Actions::Actions action)
{
    m_actionSet.insert(action);
}

bool Tool::isActionEnabled(Actions::Actions action) const
{
    if (m_actionSet.contains(action))
        return true;
    return false;
}

bool Tool::isActionChecked(Actions::Actions action) const
{
    return false;
}

void Tool::getActions(QList<Actions::Actions> &actions) const
{
    foreach(Actions::Actions id, m_actionSet)
        actions.append(id);
}
