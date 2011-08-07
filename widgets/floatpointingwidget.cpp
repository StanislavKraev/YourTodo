#include <QLineEdit>
#include <QDoubleValidator>
#include <QLayout>

#include "floatpointingwidget.h"

FloatPointingWidget::FloatPointingWidget(QString title, QWidget *parent) :
    TitledWidget(title, parent)
{
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setValidator(new QDoubleValidator(0.0, 1000000000.0, 8, m_lineEdit));
    layout()->addWidget(m_lineEdit);
    connect(m_lineEdit, SIGNAL(textChanged(QString)), SLOT(textChanged(QString)));
}

void FloatPointingWidget::setData(const QVariant &data)
{
    bool prevBlock = m_lineEdit->blockSignals(true);
    m_lineEdit->setText(QString("%1").arg(data.toDouble()));
    m_lineEdit->blockSignals(prevBlock);
}

QVariant FloatPointingWidget::data() const
{
    return m_lineEdit->text().toDouble();
}

void FloatPointingWidget::clear()
{
    m_lineEdit->setText("");
}

void FloatPointingWidget::textChanged(QString value)
{
    bool prevBlock = m_lineEdit->blockSignals(true);
    onDataChanged(value.toDouble());
    m_lineEdit->blockSignals(prevBlock);
}
