#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H

class QSpinBox;
#include "titledwidget.h"

class SpinnerWidget : public TitledWidget
{
    Q_OBJECT
public:
    explicit SpinnerWidget(QString title, int min, int max, QWidget *parent = 0);

signals:

public slots:
private:
    QSpinBox *m_spinner;
};

#endif // SPINNERWIDGET_H
