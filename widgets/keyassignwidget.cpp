#include <QKeyEvent>
#include <QDebug>

#include "keyassignwidget.h"

KeyAssignWidget::KeyAssignWidget(QWidget *parent) :
    QLineEdit(parent),
    m_ctrl(false),
    m_alt(false),
    m_key(-1)
{
}

bool KeyAssignWidget::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent *)event;

        int keyInt = keyEvent->key();
        Qt::Key key = static_cast<Qt::Key>(keyInt);
        if(key == Qt::Key_unknown){
            qDebug() << "Unknown key from a macro probably";
            return true;
        }
        // the user have clicked just and only the special keys Ctrl, Shift, Alt, Meta.
        if(key == Qt::Key_Control ||
            key == Qt::Key_Shift ||
            key == Qt::Key_Alt ||
            key == Qt::Key_Meta)
        {
            qDebug() << "Single click of special key: Ctrl, Shift, Alt or Meta";
            qDebug() << "New KeySequence:" << QKeySequence(keyInt).toString(QKeySequence::NativeText);
            return true;
        }

        // check for a combination of user clicks
        Qt::KeyboardModifiers modifiers = keyEvent->modifiers();
        QString keyText = keyEvent->text();
        // if the keyText is empty than it's a special key like F1, F5, ...
        qDebug() << "Pressed Key:" << keyText;

        QList<Qt::Key> modifiersList;
        if(modifiers & Qt::ShiftModifier)
            keyInt += Qt::SHIFT;
        if(modifiers & Qt::ControlModifier)
            keyInt += Qt::CTRL;
        if(modifiers & Qt::AltModifier)
            keyInt += Qt::ALT;
        if(modifiers & Qt::MetaModifier)
            keyInt += Qt::META;

        setText(QKeySequence(keyInt).toString(QKeySequence::NativeText));
        return true;
    }
    // http://www.qtcentre.org/archive/index.php/t-28754.html
    return QWidget::event(event);
}
