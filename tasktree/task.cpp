#include "utils.h"
#include "itasklist.h"
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

Task::Task() : m_id(-1), m_iconIndex(-1), m_pos(-1), m_priority(-1), m_risk(-1), m_percentDone(-1),
    m_cost(0.0), m_commentsType(Task::UNDEFINED), m_taskList(0)
{
}

Task::Task(int id, QString title, unsigned short percentDone, QString comments,
           CommentsType commentsType, double cost, QDateTime creationDate,
           QDateTime doneDate, int iconIndex, QDateTime lastMod, int pos,
           int priority, QColor priorityColor, int risk,
           QDateTime startDate, QColor textColor) :
    m_id(id),
    m_title(title),
    m_iconIndex(iconIndex),
    m_comments(comments),
    m_pos(pos),
    m_priority(priority),
    m_risk(risk),
    m_percentDone(percentDone),
    m_cost(cost),
    m_doneDate(doneDate),
    m_startDate(startDate),
    m_creationDate(creationDate),
    m_lastMod(lastMod),
    m_commentsType(commentsType),
    m_textColor(textColor),
    m_priorityColor(priorityColor),
    m_taskList(0)
{
}

Task::Task(int id,
           QString title) :
    m_id(id),
    m_title(title),
    m_iconIndex(-1),
    m_pos(-1),
    m_priority(5),
    m_risk(0),
    m_percentDone(0),
    m_cost(0.0),
    m_creationDate(QDateTime::currentDateTime()),
    m_lastMod(QDateTime::currentDateTime()),
    m_commentsType(UNDEFINED),
    m_taskList(0)
{
}

Task::Task(const Task &task) :
    m_subTaskList(task.m_subTaskList),
    m_id(task.m_id),
    m_title(task.m_title),
    m_iconIndex(task.m_iconIndex),
    m_comments(task.m_comments),
    m_pos(task.m_pos),
    m_priority(task.m_priority),
    m_risk(task.m_risk),
    m_percentDone(task.m_percentDone),
    m_cost(task.m_cost),
    m_doneDate(task.m_doneDate),
    m_startDate(task.m_startDate),
    m_creationDate(task.m_creationDate),
    m_lastMod(task.m_lastMod),
    m_commentsType(task.m_commentsType),
    m_textColor(task.m_textColor),
    m_priorityColor(task.m_priorityColor),
    m_parent(task.m_parent),
    m_taskList(task.m_taskList)
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

void Task::addTask(Ptr task, bool touch)
{
    m_subTaskList.append(task);
    if (touch)
        this->touch();
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
    bool changed = title != m_title;
    m_title = title;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::Title, this);
        touch();
    }
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
    bool changed = percentDone != m_percentDone;
    m_percentDone = percentDone;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::PercentDone, this);
        touch();
    }
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
    touch();
}

void Task::removeAt(int pos)
{
    if (pos < 0 || pos > count() - 1)
        return;
    m_subTaskList.removeAt(pos);
    touch();
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
        oldItem->m_taskList = newItem->m_taskList;

        foreach (Task::Ptr subTask, oldItem->m_subTaskList)
            subTask->setParent(oldItem);

        newItem->m_id = oldId;
        touch();
    }
}

QString Task::comments() const
{
    return m_comments;
}

void Task::setComments(QString comments)
{
    bool changed = m_comments != comments;
    m_comments = comments;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::Comments, this);
        touch();
    }
}

int Task::priority() const
{
    return m_priority;
}

void Task::setPriority(int priority)
{
    if (priority < 0 || priority > m_priorityColors.count() - 1)
        return;
    bool changed = m_priority != priority;
    m_priority = priority;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::Priority, this);
        touch();
    }
}

int Task::iconIndex() const
{
    return m_iconIndex;
}

void Task::setIconIndex(int iconIndex)
{
    bool changed = m_iconIndex != iconIndex;
    m_iconIndex = iconIndex;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::IconIndex, this);
        touch();
    }
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
    bool changed = m_commentsType != type;
    m_commentsType = type;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::CommentsType, this);
        touch();
    }
}

double Task::cost() const
{
    return m_cost;
}

void Task::setCost(double val)
{
    bool changed = m_cost != val;
    m_cost = val;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::Cost, this);
        touch();
    }
}

QDateTime Task::creationDate() const
{
    return m_creationDate;
}

QDateTime Task::startDate() const
{
    return m_startDate;
}

void Task::setStartDate(QDateTime val)
{
    bool changed = m_startDate != val;
    m_startDate = val;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::StartDate, this);
        touch();
    }
}

QDateTime Task::doneDate() const
{
    return m_doneDate;
}

void Task::setDoneDate(QDateTime val)
{
    bool changed = m_doneDate != val;
    m_doneDate = val;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::DoneDate, this);
        touch();
    }
}

QDateTime Task::lastModDate() const
{
    return m_lastMod;
}

int Task::posAttr() const
{
    // TODO: return calculated value.
    return m_pos;
}

QColor Task::priorityColor() const
{
    if (m_priority >= 0 && m_priority < m_priorityColors.count())
        return m_priorityColors[m_priority];
    return QColor();
}

int Task::risk() const
{
    return m_risk;
}

void Task::setRisk(int val)
{
    bool changed = m_risk != val;
    m_risk = val;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::Risk, this);
        touch();
    }
}

QColor Task::textColor() const
{
    return m_textColor;
}

void Task::setTextColor(QColor val)
{
    bool changed = m_textColor != val;
    m_textColor = val;
    if (changed && m_taskList)
    {
        m_taskList->notifyMemberChange(nsTaskData::TextColor, this);
        touch();
    }
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
    case nsTaskData::TextColor:
        return m_textColor;
    case nsTaskData::InvalidTaskMember:
        return QVariant();
    default:
        return QVariant();
    }

    return QVariant();
}

bool Task::editable(nsTaskData::TaskDataMember member) const
{
    switch(member)
    {
    case nsTaskData::PercentDone:
    case nsTaskData::Cost:
        return m_subTaskList.count() == 0;
    default:
        break;
    }
    return true;
}

void Task::setMemberData(nsTaskData::TaskDataMember member, QVariant data)
{
    bool changed(false);

    switch(member)
    {
    case nsTaskData::Title:
        changed = m_title != data.toString();
        m_title = data.toString();
        break;
    case nsTaskData::Priority:
        changed = m_priority != data.toInt();
        m_priority = data.toInt();
        break;
    case nsTaskData::PercentDone:
        changed = m_percentDone != data.toUInt();
        m_percentDone = data.toUInt();
        break;
    case nsTaskData::IconIndex:
        changed = m_iconIndex != data.toInt();
        m_iconIndex = data.toInt();
        break;
    case nsTaskData::Risk:
        changed = m_risk != data.toInt();
        m_risk = data.toInt();
        break;
    case nsTaskData::Cost:
        changed = m_cost != data.toDouble();
        m_cost = data.toDouble();
        break;
    case nsTaskData::StartDate:
        changed = m_startDate != data.toDateTime();
        m_startDate = data.toDateTime();
        break;
    case nsTaskData::DoneDate:
        changed = m_doneDate != data.toDateTime();
        m_doneDate = data.toDateTime();
        break;
    case nsTaskData::CreationDate:
        changed = m_creationDate != data.toDateTime();
        m_creationDate = data.toDateTime();
        break;
    case nsTaskData::CommentsType:
        changed = m_commentsType != (Task::CommentsType)data.toInt();
        m_commentsType = (Task::CommentsType)data.toInt();
        break;
    case nsTaskData::Comments:
        changed = m_comments != data.toString();
        m_comments = data.toString();
        break;
    case nsTaskData::TextColor:
        changed = m_textColor != data.value<QColor>();
        m_textColor = data.value<QColor>();
        break;
    default:
        return;
    }
    m_taskList->notifyMemberChange(member, this);
    if (changed)
        touch();
}

void Task::setTaskList(ITaskList *taskList)
{
    m_taskList = taskList;
}

void Task::touch()
{
    m_lastMod = QDateTime::currentDateTime();
    if (m_taskList)
        m_taskList->notifyMemberChange(nsTaskData::LastModified, this);
}
