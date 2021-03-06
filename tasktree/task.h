#ifndef TASK_H
#define TASK_H

#include "QColor"
#include "QList"
#include "QString"
#include "QDateTime"
#include "QSharedPointer"
#include "QVariant"

#include "tasktree/treecolumndata.h"

class ITaskList;

class Task
{
public:
    enum CommentsType
    {
        UNDEFINED = 0,
        PLAIN_TEXT = 1
    };
static QList<QColor> m_priorityColors;
public:
    typedef QSharedPointer<Task> Ptr;
    typedef QList<Ptr> List;
    typedef List::Iterator ListIterator;
    typedef List::ConstIterator ListConstIterator;
public:
    Task();
    Task(int id, QString title, unsigned short percentDone, QString comments,
         CommentsType commentsType, double cost, QDateTime creationDate,
         QDateTime doneDate, int iconIndex, QDateTime lastMod, int pos,
         int priority, QColor priorityColor, int risk,
         QDateTime startDate, QColor textColor);
    Task(int id, QString title);
    Task(const Task& task);
public:
    QList<Task::Ptr>::Iterator tasks();
    QList<Task::Ptr>::ConstIterator tasks() const;
    Task::Ptr getAt(int index) const;
    int pos(Task::Ptr subTask) const;
    Task::Ptr parent() const;
    void setParent(Task::Ptr parent);
    int count() const;
    int id() const;
    void replace(Ptr oldItem, Ptr newItem);
    int calcPosAttr() const;
    double calcCost() const;
    QVariant memberData(nsTaskData::TaskDataMember member) const;
    void setMemberData(nsTaskData::TaskDataMember member, QVariant data);
    bool editable(nsTaskData::TaskDataMember member) const;
    void setTaskList(ITaskList *taskList);
public:
    QString title() const;
    void setTitle(QString title);
    unsigned short percentDone() const;
    void setPercentDone(unsigned short percentDone);
    void toggleDone(unsigned short percentDone);
    QString comments() const;
    void setComments(QString comments);
    int priority() const;
    void setPriority(int priority);
    int iconIndex() const;
    void setIconIndex(int iconIndex);

    CommentsType commentsType() const;
    void setCommentsType(CommentsType type);
    double cost() const;
    void setCost(double val);
    QDateTime creationDate() const;
    QDateTime startDate() const;
    void setStartDate(QDateTime val);
    QDateTime doneDate() const;
    void setDoneDate(QDateTime val);
    QDateTime lastModDate() const;
    int posAttr() const;
    QColor priorityColor() const;
    int risk() const;
    void setRisk(int val);
    QColor textColor() const;
    void setTextColor(QColor val);
private:
    void addTask(Ptr task, bool touch = true);
    void insertSubTask(int pos, Ptr task);
    void removeAt(int pos);
    unsigned short calculatePercentDone() const;
    void touch();
private:
    QList<Ptr> m_subTaskList;
    int m_id;   // unique
    QString m_title;
    int m_iconIndex;
    QString m_comments;
    int m_pos;
    int m_priority;
    int m_risk;
    unsigned short m_percentDone;
    double m_cost;
    QDateTime m_doneDate;
    QDateTime m_startDate;
    QDateTime m_creationDate;
    QDateTime m_lastMod;
    CommentsType m_commentsType;
    QColor m_textColor;
    QColor m_priorityColor;
    Task::Ptr m_parent;
    ITaskList* m_taskList;

friend class TaskList;
};

Q_DECLARE_METATYPE(Task::Ptr);

#endif // TASK_H
