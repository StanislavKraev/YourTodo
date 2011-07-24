#include "utils.h"
#include "task.h"

QList<QColor> Task::m_priorityColors;
class InitColors
{
public:
    InitColors()
    {
        Task::m_priorityColors << QColor("#1ee100")
                            << QColor("#00e43f")
                            << QColor("#00e7a0")
                            << QColor("#00d1ea")
                            << QColor("#0071ed")
                            << QColor("#000ff0")
                            << QColor("#5400f3")
                            << QColor("#ba00f6")
                            << QColor("#f900cd")
                            << QColor("#fc0068")
                            << QColor("#ff0000");
    }
};
static InitColors initColors;

Task::Task() : m_id(-1)
{
}

Task::Task(int id, QString title, unsigned short percentDone, QString comments,
           CommentsType commentsType, double cost, QDateTime creationDate,
           QDateTime doneDate, int iconIndex, QDateTime lastMod, int pos,
           int priority, QColor priorityColor, int risk,
           QDateTime startDate, QColor textColor) :
    m_id(id),
    m_title(title),
    m_percentDone(percentDone),
    m_comments(comments),
    m_commentsType(UNDEFINED),
    m_cost(cost),
    m_creationDate(creationDate),
    m_doneDate(doneDate),
    m_startDate(startDate),
    m_lastMod(lastMod),
    m_iconIndex(iconIndex),
    m_pos(pos),
    m_priority(priority),
    m_priorityColor(priorityColor),
    m_risk(risk),
    m_textColor(textColor)
{
}

Task::Task(int id,
           QString title) :
    m_id(id),
    m_title(title),
    m_percentDone(0),
    m_commentsType(UNDEFINED),
    m_cost(0.0),
    m_creationDate(QDateTime::currentDateTime()),
    m_lastMod(QDateTime::currentDateTime()),
    m_iconIndex(-1),
    m_pos(-1),
    m_priority(5),
    m_risk(0)
{
}

Task::Task(const Task &task) :
    WatchObserver(task),
    m_id(task.m_id),
    m_title(task.m_title),
    m_subTaskList(task.m_subTaskList),
    m_parent(task.m_parent),
    m_percentDone(task.m_percentDone)
  // , TODO:
{
}

QList<Task::Ptr>::Iterator Task::tasks()
{
    return m_subTaskList.begin();
}

QList<Task::Ptr>::ConstIterator Task::tasks() const
{
    return m_subTaskList.begin();
}

void Task::addTask(Ptr task)
{
    m_subTaskList.append(task);
}

int Task::id() const
{
    return m_id;
}

Task::Ptr Task::getAt(int index) const
{
    return m_subTaskList[index];
}

QString Task::title() const
{
    return m_title;
}

int Task::count() const
{
    return m_subTaskList.count();
}

int Task::pos(Task::Ptr subTask) const
{
    return m_subTaskList.indexOf(subTask);
}

Task::Ptr Task::parent() const
{
    return m_parent;
}

void Task::setParent(Task::Ptr parent)
{
    m_parent = parent;
}

void Task::setTitle(QString title)
{
    m_title = title;
}

unsigned short Task::percentDone() const
{
    if (m_subTaskList.count() > 0)
        return calculatePercentDone();

    return m_percentDone;
}

void Task::setPercentDone(unsigned short percentDone)
{
    if (m_subTaskList.count() > 0)
        return;
    m_percentDone = percentDone;
}

void Task::toggleDone(unsigned short percentDone)
{
    foreach (Task::Ptr task, m_subTaskList)
        task->toggleDone(percentDone);
    m_percentDone = percentDone;
}

void Task::insertSubTask(int pos, Task::Ptr task)
{
    if (pos < 0 ||
        ((pos > count()) && (count() > 0)))
        return;
    m_subTaskList.insert(pos, task);
}

void Task::removeAt(int pos)
{
    if (pos < 0 || pos > count() - 1)
        return;
    m_subTaskList.removeAt(pos);
}

void Task::replace(Task::Ptr oldItem, Task::Ptr newItem)
{
    if (m_subTaskList.contains(oldItem))
    {
        int oldId = oldItem->m_id;
        oldItem->m_comments = newItem->m_comments;
        oldItem->m_commentsType = newItem->m_commentsType;
        oldItem->m_cost = newItem->m_cost;
        oldItem->m_creationDate = newItem->m_creationDate;
        oldItem->m_doneDate = newItem->m_doneDate;
        oldItem->m_iconIndex = newItem->m_iconIndex;
        oldItem->m_id = newItem->m_id;
        oldItem->m_lastMod = newItem->m_lastMod;
        oldItem->m_percentDone = newItem->m_percentDone;
        oldItem->m_pos = newItem->m_pos;
        oldItem->m_priority = newItem->m_priority;
        oldItem->m_priorityColor = newItem->m_priorityColor;
        oldItem->m_risk = newItem->m_risk;
        oldItem->m_startDate = newItem->m_startDate;
        oldItem->m_subTaskList = newItem->m_subTaskList;
        oldItem->m_textColor = newItem->m_textColor;
        oldItem->m_title = newItem->m_title;

        foreach (Task::Ptr subTask, oldItem->m_subTaskList)
            subTask->setParent(oldItem);

        newItem->m_id = oldId;
    }
}

QString Task::comments() const
{
    return m_comments;
}

void Task::setComments(QString comments)
{
    m_comments = comments;
}

int Task::priority() const
{
    return m_priority;
}

void Task::setPriority(int priority)
{
    if (priority < 0 || priority > m_priorityColors.count() - 1)
        return;
    m_priority = priority;
}

int Task::iconIndex() const
{
    return m_iconIndex;
}

void Task::setIconIndex(int iconIndex)
{
    m_iconIndex = iconIndex;
}

unsigned short Task::calculatePercentDone() const
{
    int sum = 0;
    foreach (Task::Ptr subTask, m_subTaskList)
    {
        sum += subTask->percentDone();
    }
    return (unsigned short)((double)sum / (double)m_subTaskList.count());
}

Task::CommentsType Task::commentsType() const
{
    return m_commentsType;
}

void Task::setCommentsType(Task::CommentsType type)
{
    m_commentsType = type;
}

double Task::cost() const
{
    return m_cost;
}

void Task::setCost(double val)
{
    m_cost = val;
}

QDateTime Task::creationDate() const
{
    return m_creationDate;
}

void Task::setCreationDate(QDateTime val)
{
    m_creationDate = val;
}

QDateTime Task::startDate() const
{
    return m_startDate;
}

void Task::setStartDate(QDateTime val)
{
    m_startDate = val;
}

QDateTime Task::doneDate() const
{
    return m_doneDate;
}

void Task::setDoneDate(QDateTime val)
{
    m_doneDate = val;
}

QDateTime Task::lastModDate() const
{
    return m_lastMod;
}

void Task::setLastModDate(QDateTime val)
{
    m_lastMod = val;
}

int Task::posAttr() const
{
    return m_pos;
}

void Task::setPosAttr(int val)
{
    m_pos = val;
}

QColor Task::priorityColor() const
{
    if (m_priority >= 0 && m_priority < m_priorityColors.count())
        return m_priorityColors[m_priority];
    return QColor();
}

void Task::setPriorityColor(QColor val)
{
    m_priorityColor = val;
}

int Task::risk() const
{
    return m_risk;
}

void Task::setRisk(int val)
{
    m_risk = val;
}

QColor Task::textColor() const
{
    return m_textColor;
}

void Task::setTextColor(QColor val)
{
    m_textColor = val;
}

int Task::calcPosAttr() const
{
    int i = 0;
    Task::Ptr current = parent();
    while (current)
    {
        i++;
        current = current->parent();
    }
    return i;
}

QDateTime Task::dueDate() const
{
    return QDateTime();
}

void Task::setDueDate(QDateTime date)
{
}

double Task::calcCost() const
{
    double sum = 0;
    foreach (Task::Ptr subTask, m_subTaskList)
    {
        sum += subTask->calcCost();
    }
    return cost() + sum;
}

QVariant Task::memberData(nsTaskData::TaskDataMember member) const
{
    switch(member)
    {
    case nsTaskData::Title:
        return m_title;
    case nsTaskData::Priority:
        return m_priority;
    case nsTaskData::PercentDone:
        return m_percentDone;
    case nsTaskData::IconIndex:
        return m_iconIndex;
    case nsTaskData::Position:
        return m_pos;
    case nsTaskData::Risk:
        return m_risk;
    case nsTaskData::Cost:
        return m_cost;
    case nsTaskData::StartDate:
        return m_startDate;
    case nsTaskData::DoneDate:
        return m_doneDate;
    case nsTaskData::CreationDate:
        return m_creationDate;
    case nsTaskData::LastModified:
        return m_lastMod;
    case nsTaskData::CommentsType:
        return m_commentsType;
    case nsTaskData::Comments:
        return m_comments;
    }

    return QVariant();
}

bool Task::editable(nsTaskData::TaskDataMember member) const
{
    switch(member)
    {
    case nsTaskData::PercentDone:
        return m_subTaskList.count() == 0;
    }
    return true;
}

void Task::setMemberData(nsTaskData::TaskDataMember member, QVariant data)
{
    switch(member)
    {
    case nsTaskData::Title:
        m_title = data.toString();
        break;
    case nsTaskData::Priority:
        m_priority = data.toInt();
        break;
    case nsTaskData::PercentDone:
        m_percentDone = data.toUInt();
        break;
    case nsTaskData::IconIndex:
        m_iconIndex = data.toInt();
        break;
    case nsTaskData::Position:
        m_pos = data.toInt();
        break;
    case nsTaskData::Risk:
        m_risk = data.toInt();
        break;
    case nsTaskData::Cost:
        m_cost = data.toDouble();
        break;
    case nsTaskData::StartDate:
        m_startDate = data.toDateTime();
        break;
    case nsTaskData::DoneDate:
        m_doneDate = data.toDateTime();
        break;
    case nsTaskData::CreationDate:
        m_creationDate = data.toDateTime();
        break;
    case nsTaskData::LastModified:
        m_lastMod = data.toDateTime();
        break;
    case nsTaskData::CommentsType:
        m_commentsType = (Task::CommentsType)data.toInt();
        break;
    case nsTaskData::Comments:
        m_comments = data.toString();
        break;
    }
    notifyMemberChange(member, this);
}
