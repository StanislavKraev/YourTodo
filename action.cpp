#include "QAction"

#include "action.h"

Action::Action() : m_action(0), m_id(Actions::Undefined)
{

}

Action::~Action()
{

}

Actions::Actions Action::id() const
{
    return m_id;
}

bool Action::enabled() const
{
    return m_action ? m_action->isEnabled() : false;
}

void Action::setEnabled(bool val)
{
    if (m_action)
        m_action->setEnabled(val);
}

