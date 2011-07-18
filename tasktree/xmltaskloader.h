#ifndef XMLTASKLOADER_H
#define XMLTASKLOADER_H

#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QString>
#include <QDateTime>

#include "itaskloader.h"

class XmlTaskLoader : public ITaskLoader
{
public:
    XmlTaskLoader(QString fileName);
    virtual ~XmlTaskLoader();
public:
    virtual bool readHeader(QString &projectName, int &fileFormat,
                                int &uniqueId, int &fileVersion, QDateTime &earliestDueDate);
    virtual TaskInfo read();
    virtual QString fileName() const;
private:
    QDomDocument *m_document;
    QString m_fileName;
    QDomElement m_curElement;
    int m_level;
};

#endif // XMLTASKLOADER_H
