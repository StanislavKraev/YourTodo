#include <QSpinBox>
#include <QLayout>

#include "spinnerwidget.h"

SpinnerWidget::SpinnerWidget(QString title, int min, int max, QWidget *parent) :
    TitledWidget(title, parent)
{
    m_spinner = new QSpinBox(this);
    m_spinner->setRange(min, max);
    m_spinner->setMinimumWidth(80);
    layout()->addWidget(m_spinner);
}

void SpinnerWidget::setData(const QVariant &data)
{
}

QVariant SpinnerWidget::data() const
{
    return QVariant();
}

void SpinnerWidget::clear()
{
    m_spinner->setValue(m_spinner->minimum());
}