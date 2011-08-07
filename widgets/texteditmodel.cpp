#include <QTextEdit>

#include "texteditmodel.h"

TextEditModel::TextEditModel(QTextEdit *control) :
    QObject(control),
    m_control(control)
{
    m_control->setEnabled(false);
    connect(m_control, SIGNAL(textChanged()), SLOT(textChanged()));
}

void TextEditModel::textChanged()
{
    if (m_task)
        m_task->setComments(m_control->toPlainText());
}

void TextEditModel::taskChanged(nsTaskData::TaskDataMember member, Task *task)
{
    if (!task || (task != m_task) || (member != nsTaskData::Comments))
        return;
    bool block = m_control->blockSignals(true);
    m_control->setText(m_task->comments());
    m_control->blockSignals(block);
}

void TextEditModel::setTask(Task::Ptr task)
{
    m_task = task;
    m_control->setEnabled(m_task);
    if (m_task)
    {
        bool block = m_control->blockSignals(true);
        m_control->setText(m_task->comments());
        m_control->blockSignals(block);
    }
    else
    {
        m_control->setText("");
    }
}
