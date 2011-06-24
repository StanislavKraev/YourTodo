#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

class Exception
{
public:
    Exception();
    Exception(QString msg);
public:
    virtual QString message();
private:
    QString m_msg;
};

#endif // EXCEPTION_H
