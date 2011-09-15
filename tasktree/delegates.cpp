#include <QTreeView>
#include <QHeaderView>
#include <QApplication>
#include <QPixmap>
#include <QPainter>

#include "task.h"

#include "delegates.h"

PriorityDelegate::PriorityDelegate(QTreeView *treeView)
{
    int gridHint = treeView->style()->styleHint(QStyle::SH_Table_GridLineColor,
                                                new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    m_gridPen = QPen(gridColor);
}


void PriorityDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QVector<QLine> lines;
    lines << QLine(QPoint(opt.rect.left(), opt.rect.bottom()),
                   QPoint(opt.rect.right(), opt.rect.bottom()))
          << QLine(QPoint(opt.rect.right(), opt.rect.top()),
                   QPoint(opt.rect.right(), opt.rect.bottom()));

    painter->setPen(m_gridPen);
    painter->drawLines(lines);

    Task::Ptr task = index.data(Qt::UserRole).value<Task::Ptr>();
    if (task->percentDone() >= 100)
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
    else
    {
        opt.rect.setWidth(opt.rect.width() - 2);
        opt.rect.setHeight(opt.rect.height() - 2);
        opt.rect.setLeft(opt.rect.left() + 1);
        opt.rect.setTop(opt.rect.top() + 1);
        QStyle *style = QApplication::style();
        QColor color = task->priorityColor();
        if (color.isValid())
        {
            opt.backgroundBrush = color;
            opt.displayAlignment = Qt::AlignCenter;
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, 0);
        }
    }
}

IconIndexDelegate::IconIndexDelegate(QTreeView *treeView)
{
    int gridHint = treeView->style()->styleHint(QStyle::SH_Table_GridLineColor, new QStyleOptionViewItemV4());
    QColor gridColor = static_cast<QRgb>(gridHint);
    m_gridPen = QPen(gridColor);
    m_directoryIcon = treeView->style()->standardIcon(QStyle::SP_DirIcon);

    m_iconListPixmap.load(":/icons/task_icons.png");
}

void IconIndexDelegate::paint(QPainter *painter,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QVector<QLine> lines;
    lines << QLine(QPoint(opt.rect.left(), opt.rect.bottom()),
                   QPoint(opt.rect.right(), opt.rect.bottom()))
          << QLine(QPoint(opt.rect.right(), opt.rect.top()),
                   QPoint(opt.rect.right(), opt.rect.bottom()));

    painter->setPen(m_gridPen);
    painter->drawLines(lines);

    Task::Ptr task = index.data(Qt::UserRole).value<Task::Ptr>();
    int icon_index = task->iconIndex();
    if (icon_index >= 0)
    {
        icon_index -= 1;
        int left = icon_index * 16;
        painter->drawPixmap(opt.rect.left() + 1, opt.rect.top() + 1, m_iconListPixmap,  left, 0, 16, 16);
    }
    else if (task->count())
    {
        painter->drawPixmap(opt.rect.left() + 1, opt.rect.top() + 1, m_directoryIcon.pixmap(16, 16));
    }
}

PriorityComboBoxDelegate::PriorityComboBoxDelegate()
{
    QColor gridColor = Qt::gray;
    m_gridPen = QPen(gridColor);
}


void PriorityComboBoxDelegate::paint(QPainter *painter,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const
{
    QStyleOptionViewItemV4 opt = option;
    initStyleOption(&opt, index);

    QVector<QLine> lines;
    lines << QLine(QPoint(opt.rect.left(), opt.rect.bottom()),
                   QPoint(opt.rect.right(), opt.rect.bottom()))
          << QLine(QPoint(opt.rect.right(), opt.rect.top()),
                   QPoint(opt.rect.right(), opt.rect.bottom()));

    int id = index.row();

//    painter->setPen(m_gridPen);
    if (id > 0)
    {
        painter->drawLines(lines);
        QColor color = Task::m_priorityColors[id - 1];
        painter->setPen(color);
    }

    //Task::Ptr task = index.data(Qt::UserRole).value<Task::Ptr>();
    //if (id > 0)
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
    /*else
    {
        opt.rect.setWidth(opt.rect.width() - 2);
        opt.rect.setHeight(opt.rect.height() - 2);
        opt.rect.setLeft(opt.rect.left() + 1);
        opt.rect.setTop(opt.rect.top() + 1);
        QStyle *style = QApplication::style();
        opt.backgroundBrush = task->priorityColor();
        opt.displayAlignment = Qt::AlignCenter;
        style->drawControl(QStyle::CE_ComboBoxLabel, &opt, painter, 0);
    }*/
}


TitleDelegate::TitleDelegate(QTreeView *treeView)
{
}


void TitleDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QStyledItemDelegate::paint(painter, option, index);
}
