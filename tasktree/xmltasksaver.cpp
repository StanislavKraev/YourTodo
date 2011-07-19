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
    m_curElement.setAttribute("FILEVERSION", fileVersion + 1);
    m_curElement.setAttribute("EARLIESTDUEDATE", toOleTime(earliestDueDate));
}

void XmlTaskSaver::save(TaskInfo info)
{
    QDomElement newElement = m_document->createElement("TASK");
    m_lastElement = m_curElement.appendChild(newElement).toElement();

    m_lastElement.setAttribute("ID", info.id);
    m_lastElement.setAttribute("TITLE", info.title);
    m_lastElement.setAttribute("PERCENTDONE", info.percentDone);
    m_lastElement.setAttribute("ICONINDEX", info.iconIndex);
    m_lastElement.setAttribute("POS", info.pos);
    m_lastElement.setAttribute("PRIORITY", info.priority);
    m_lastElement.setAttribute("RISK", info.risk);
    m_lastElement.setAttribute("COST", info.cost);
    if (info.startDate > 10.0)
        m_lastElement.setAttribute("STARTDATE", info.startDate);
    m_lastElement.setAttribute("CREATIONDATE", info.creationDate);
    if (info.doneDate > 10.0)
        m_lastElement.setAttribute("DONEDATE", info.doneDate);
    m_lastElement.setAttribute("PRIORITYCOLOR", info.priorityColor);
    m_lastElement.setAttribute("TEXTCOLOR", info.textColor);
    m_lastElement.setAttribute("COMMENTS", info.comments);
    m_lastElement.setAttribute("COMMENTSTYPE", info.commentsType);
    m_lastElement.setAttribute("LASTMOD", info.lastMod);
}

void XmlTaskSaver::goDown()
{
    m_curElement = m_lastElement;
}

void XmlTaskSaver::goUp()
{
    m_curElement = m_curElement.parentNode().toElement();
}
