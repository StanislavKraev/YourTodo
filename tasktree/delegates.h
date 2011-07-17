#ifndef DELEGATES_H
#define DELEGATES_H

#include <QStyledItemDelegate>
#include <QPen>
class QTreeView;
class QPainter;
class QModelIndex;
class QStyleOptionViewItem;


class PriorityDelegate : public QStyledItemDelegate
{
private:
    QPen m_gridPen;
public:
    PriorityDelegate(QTreeView *treeView);
public:
    virtual void paint(QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class IconIndexDelegate : public QStyledItemDelegate
{
private:
    QPen m_gridPen;
    QIcon m_directoryIcon;
    QPixmap m_iconListPixmap;
public:
    IconIndexDelegate(QTreeView *treeView);
public:
    virtual void paint(QPainter *painter,
            const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


#endif // DELEGATES_H
