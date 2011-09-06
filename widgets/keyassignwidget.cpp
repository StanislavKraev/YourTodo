#include <QKeyEvent>
#include <QDebug>

#include "keyassignwidget.h"

KeyAssignWidget::KeyAssignWidget(QWidget *parent) :
    QLineEdit(parent),
    m_ctrl(false),
    m_alt(false),
    m_shift(false),
    m_meta(false),
    m_key(-1)
{
}

bool KeyAssignWidget::event(QEvent *event)
{
    QKeyEvent *keyEvent = 0;
    bool pressed(false);
    if (event->type() == QEvent::KeyPress)
    {
        keyEvent = (QKeyEvent *)event;
        pressed = true;
    }
    else if (event->type() == QEvent::KeyRelease)
    {
        keyEvent = (QKeyEvent *)event;
        pressed = false;
    }

    if (keyEvent)
    {
        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);
        if(key == Qt::Key_unknown)
        {
            event->ignore();
            return true;
        }

        if (key == Qt::Key_Control)
            m_ctrl = pressed;
        else if (key == Qt::Key_Alt)
            m_alt = pressed;
        else if (key == Qt::Key_Shift)
            m_shift = pressed;
        else if (key == Qt::Key_Meta)
            m_meta = pressed;
        else
        {
            m_key = pressed ? keyInt : -1;
        }

        event->accept();
        bool final(false);
        QString res = makeText(final);
        setText(res);
        if (final)
        {
            m_sequence = QKeySequence(res);
            emit(assigned(m_sequence));
            m_ctrl = false;
            m_alt = false;
            m_shift = false;
            m_meta = false;
            m_key = -1;
        }
        return true;
    }
    return QWidget::event(event);
}

QString KeyAssignWidget::makeText(bool &final) const
{
    QString text;
    if (m_ctrl)
        text += "Ctrl+";
    if (m_shift)
        text += "Shift+";
    if (m_alt)
        text += "Alt+";
    if (m_meta)
        text += "Meta+";
    QString keyVal;
    if (m_key >= 0)
    {
        keyVal = QKeySequence(m_key).toString(QKeySequence::NativeText);
        text += keyVal;
    }
    final = keyVal.length() > 0;
    return text;
}

void KeyAssignWidget::setSequence(QKeySequence seq)
{
    m_sequence = seq;
    setText(m_sequence.toString(QKeySequence::NativeText));
}
