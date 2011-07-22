#include <QLabel>
#include <QHBoxLayout>

#include "titledwidget.h"

TitledWidget::TitledWidget(QString title, QWidget *parent) :
    QWidget(parent)
{
    m_layout = new QHBoxLayout();
    this->setLayout(m_layout);
    m_label = new QLabel(title, parent);
    m_layout->addWidget(m_label);
}
