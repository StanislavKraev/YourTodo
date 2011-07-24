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
    int percent = data.toInt();
    if (percent < m_spinner->minimum())
        percent = m_spinner->minimum();
    if (percent > m_spinner->maximum())
        percent = m_spinner->maximum();
    m_spinner->setValue(percent);
}

QVariant SpinnerWidget::data() const
{
    return m_spinner->value();
}

void SpinnerWidget::clear()
{
    m_spinner->setValue(m_spinner->minimum());
}
