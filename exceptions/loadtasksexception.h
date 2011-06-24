#ifndef LOADTASKSEXCEPTION_H
#define LOADTASKSEXCEPTION_H

#include "exceptions/exception.h"

class LoadTasksException : public Exception
{
public:
    LoadTasksException(QString message);
};

#endif // LOADTASKSEXCEPTION_H
