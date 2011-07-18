#ifndef XMLTASKSAVER_H
#define XMLTASKSAVER_H

#include <QDomDocument>
#include <QString>

#include "tasktree/itasksaver.h"

class XmlTaskSaver : public ITaskSaver
{
public:
    XmlTaskSaver();
    virtual ~XmlTaskSaver();
public:
    virtual void saveHeader(QString projectName, int fileFormat,
                            int uniqueId, int fileVersion, QDateTime earliestDueDate);
    virtual void save(TaskInfo info);
    virtual void init(QString fileName);
    virtual void finish();
private:
    QDomDocument *m_document;
    QDomElement m_curElement;
    QString m_fileName;
};

#endif // XMLTASKSAVER_H
