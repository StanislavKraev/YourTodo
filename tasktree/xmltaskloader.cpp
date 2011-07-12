#include <QFile>

#include "exceptions/loadtasksexception.h"
#include "exceptions/stopiterexception.h"

#include "xmltaskloader.h"

XmlTaskLoader::XmlTaskLoader(QString fileName) :
    m_document(0),
    m_fileName(fileName),
    m_level(0)
{
}

XmlTaskLoader::~XmlTaskLoader()
{
    if (m_document)
    {
        delete m_document;
    }
}

TaskInfo XmlTaskLoader::read()
{
    if (!m_document)
    {
        m_document = new QDomDocument(m_fileName);
        QFile file(m_fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw LoadTasksException("Can't open tasklist file.");

        if (!m_document->setContent(&file))
        {
            file.close();
            throw LoadTasksException("Can't parse tasklist file.");
        }
        file.close();

        QDomElement docElem = m_document->documentElement();

        m_curElement = docElem.firstChildElement("TASK");
    }

    int curLevel = m_level;
    while (!m_curElement.isNull())
    {
        QDomElement taskElement = m_curElement;

        QDomElement childElement;
        if (m_curElement.hasChildNodes() &&
            !(childElement = m_curElement.firstChildElement("TASK")).isNull())
        {
            m_curElement = childElement;
            m_level++;
        }
        else
        {
            QDomElement nextElement = m_curElement.nextSiblingElement("TASK");
            if (!nextElement.isNull())
            {
                m_curElement = nextElement;
            }
            else
            {
                while (m_level > 0)
                {
                    m_curElement = m_curElement.parentNode().toElement();
                    m_level--;
                    QDomElement nextElement = m_curElement.nextSiblingElement("TASK");
                    if (!nextElement.isNull())
                    {
                        m_curElement = nextElement;
                        break;
                    }
                    if (m_level == 0)
                    {
                        m_curElement = nextElement;
                    }
                }
            }
        }

        TaskInfo taskInfo;

        // Required arguments
        bool parsedOk = false;
        taskInfo.id = taskElement.attribute("ID").toInt(&parsedOk);
        if (!parsedOk)
            throw LoadTasksException(QString("Can't parse task ID at line %1.").arg(taskElement.lineNumber()));
        taskInfo.title = taskElement.attribute("TITLE");

        // Optional arguments
        taskInfo.percentDone = taskElement.attribute("PERCENTDONE", "0").toUShort();
        taskInfo.iconIndex = taskElement.attribute("ICONINDEX", "-1").toInt();
        taskInfo.pos = taskElement.attribute("POS", "1").toInt();
        taskInfo.priority = taskElement.attribute("PRIORITY", "5").toInt();
        taskInfo.risk = taskElement.attribute("RISK", "0").toInt();
        taskInfo.cost = taskElement.attribute("COST", "0.0").toDouble();
        taskInfo.startDate = taskElement.attribute("STARTDATE", "0.0").toDouble();
        taskInfo.creationDate = taskElement.attribute("CREATIONDATE", "0.0").toDouble();
        taskInfo.doneDate = taskElement.attribute("DONEDATE", "0.0").toDouble();
        taskInfo.priorityColor = taskElement.attribute("PRIORITYCOLOR");
        taskInfo.textColor = taskElement.attribute("TEXTCOLOR");
        taskInfo.comments = taskElement.attribute("COMMENTS");
        taskInfo.commentsType = taskElement.attribute("COMMENTSTYPE");
        taskInfo.lastMod = taskElement.attribute("LASTMOD", "0.0").toDouble();

        int parentId = curLevel > 0 ? taskElement.
                                        parentNode().
                                        toElement().
                                        attribute("ID").
                                        toInt() :
                                     -1;

        taskInfo.parentId = parentId;

        return taskInfo;
    }
    throw StopIterException();
}

QString XmlTaskLoader::fileName() const
{
    return m_fileName;
}
