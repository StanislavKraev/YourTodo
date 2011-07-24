#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H

#include "titledwidget.h"

class QSpinBox;

class SpinnerWidget : public TitledWidget
{
    Q_OBJECT
public:
    explicit SpinnerWidget(QString title, int min, int max, QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data);
    virtual QVariant data() const;
    virtual void clear();

signals:

public slots:
private:
    QSpinBox *m_spinner;
};

#endif // SPINNERWIDGET_H
