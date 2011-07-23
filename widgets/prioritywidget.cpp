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
