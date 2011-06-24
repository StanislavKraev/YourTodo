#include "exception.h"

Exception::Exception()
{
}

Exception::Exception(QString msg) : m_msg(msg)
{
}

QString Exception::message()
{
    return m_msg;
}
