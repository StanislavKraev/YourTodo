#include <QFile>
#include <QTextStream>

#include "utils.h"

#include "xmltasksaver.h"

XmlTaskSaver::XmlTaskSaver() : m_document(0)
{
}

XmlTaskSaver::~XmlTaskSaver()
{
    if (m_document)
        delete m_document;
}

void XmlTaskSaver::save(TaskInfo info)
{

}

void XmlTaskSaver::init(QString fileName)
{
    m_fileName = fileName;
    m_document = new QDomDocument("xml");
}

void XmlTaskSaver::finish()
{
    if (!m_document)
        return;
    QFile file(m_fileName);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream fileStream(&file);
        m_document->save(fileStream, 2);
        file.close();
    }
    delete m_document;
    m_document = 0;
}

void XmlTaskSaver::saveHeader(QString projectName, int fileFormat, int uniqueId, int fileVersion, QDateTime earliestDueDate)
{
    if (!m_document)
        return;

    m_curElement = m_document->appendChild(m_document->createElement("TODOLIST")).toElement();

    m_curElement.setAttribute("PROJECTNAME", projectName);
    m_curElement.setAttribute("FILEFORMAT", fileFormat);
    m_curElement.setAttribute("NEXTUNIQUEID", uniqueId);
    m_curElement.setAttribute("FILEVERSION", fileVersion);
    m_curElement.setAttribute("EARLIESTDUEDATE", toOleTime(earliestDueDate));
}
