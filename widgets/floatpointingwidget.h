#ifndef FLOATPOINTINGWIDGET_H
#define FLOATPOINTINGWIDGET_H

#include "titledwidget.h"

class QLineEdit;

class FloatPointingWidget : public TitledWidget
{
    Q_OBJECT
public:
    FloatPointingWidget(QString title, QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data);
    virtual QVariant data() const;
    virtual void clear();

signals:

private slots:
    void textChanged(QString value);
private:
    QLineEdit *m_lineEdit;
};

#endif // FLOATPOINTINGWIDGET_H
