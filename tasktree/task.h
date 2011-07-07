#ifndef TASK_H
#define TASK_H

#include "QColor"
#include "QList"
#include "QString"
#include "QDateTime"
#include "QSharedPointer"
#include "QVariant"

class Task
{
public:
    enum CommentsType
    {
        UNDEFINED = 0,
        PLAIN_TEXT = 1
    };

public:
    typedef QSharedPointer<Task> Ptr;
    typedef QList<Ptr> List;
    typedef List::Iterator ListIterator;
    typedef List::ConstIterator ListConstIterator;
public:
    Task();
    Task(int id, QString title, unsigned short percentDone, QString comments,
         QString commentsType, double cost, double creationDate,
         double doneDate, int iconIndex, double lastMod, int pos,
         int priority, QString priorityColor, int risk,
         double startDate, QString textColor);
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
public:
    QString title() const;
    void setTitle(QString title);
    unsigned short percentDone() const;
    void setPercentDone(unsigned short percentDone);
    QString comments() const;
    void setComments(QString comments);
    int priority() const;
    void setPriority(int priority);
    int iconIndex() const;
    void setIconIndex(int iconIndex);
private:
    void addTask(Ptr task);
    void insertSubTask(int pos, Ptr task);
    void removeAt(int pos);
    unsigned short calculatePercentDone() const;
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

friend class TaskList;
};

Q_DECLARE_METATYPE(Task::Ptr);

#endif // TASK_H
