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
        bool parsedOk = false;
        taskInfo.id = taskElement.attribute("ID").toInt(&parsedOk);
        if (!parsedOk)
            continue; // TODO: assertion here!!!
        taskInfo.title = taskElement.attribute("TITLE");
        taskInfo.percentDone = taskElement.attribute("PERCENTDONE").toUShort(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.iconIndex = taskElement.attribute("ICONINDEX").toInt(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.pos = taskElement.attribute("POS").toInt(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.priority = taskElement.attribute("PRIORITY").toInt(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.risk = taskElement.attribute("RISK").toInt(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.cost = taskElement.attribute("COST").toDouble(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.startDate = taskElement.attribute("STARTDATE").toDouble(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.creationDate = taskElement.attribute("CREATIONDATE").toDouble(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.doneDate = taskElement.attribute("DONEDATE").toDouble(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!
        taskInfo.priorityColor = taskElement.attribute("PRIORITYCOLOR");
        taskInfo.textColor = taskElement.attribute("TEXTCOLOR");
        taskInfo.comments = taskElement.attribute("COMMENTS");
        taskInfo.commentsType = taskElement.attribute("COMMENTSTYPE");
        taskInfo.lastMod = taskElement.attribute("LASTMOD").toDouble(&parsedOk);
        if (!parsedOk)
            continue;// TODO: assertion here!!!


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
