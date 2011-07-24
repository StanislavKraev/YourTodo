#include <QComboBox>
#include <QLayout>
#include <QStringList>

#include "tasktree/delegates.h"

#include "prioritywidget.h"

PriorityWidget::PriorityWidget(QString title, QWidget *parent) :
    TitledWidget(title, parent)
{
    m_comboBox = new QComboBox();
    layout()->addWidget(m_comboBox);
    QStringList texts;
    texts << " <none >"
          << "0 (Lowest)"
          << "1 (Very Low)"
          << "2 (Low)"
          << "3 (Low-Med)"
          << "4 (Medium)"
          << "5 (Medium)"
          << "6 (Medium)"
          << "7 (Med-High)"
          << "8 (High)"
          << "9 (Very High)"
          << "10 (Highest)";
    m_comboBox->addItems(texts);
    m_comboBox->setMaxCount(20);
    m_comboBox->setMaxVisibleItems(20);
    m_comboBox->setItemDelegate(new PriorityComboBoxDelegate());
    m_comboBox->setMinimumWidth(80);
}

void PriorityWidget::setData(const QVariant &data)
{
    int priority = data.toInt();
    m_comboBox->setCurrentIndex(indexFromPriority(priority));
}

QVariant PriorityWidget::data() const
{
    return priorityFromIndex(m_comboBox->currentIndex());
}

int PriorityWidget::priorityFromIndex(int index) const
{
    if (index <= 0 || index > m_comboBox->count() - 1)
        return -1;
    return index - 1;
}

int PriorityWidget::indexFromPriority(int priority) const
{
    if (priority < 0 || priority > 10)
        return 0;
    return priority + 1;
}

void PriorityWidget::clear()
{
    m_comboBox->setCurrentIndex(0);
}
