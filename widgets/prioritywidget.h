#ifndef PRIORITYWIDGET_H
#define PRIORITYWIDGET_H

#include "titledwidget.h"

class QComboBox;

class PriorityWidget : public TitledWidget
{
    Q_OBJECT
public:
    explicit PriorityWidget(QString title, QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data);
    virtual QVariant data() const;
private:
    int priorityFromIndex(int index) const;
    int indexFromPriority(int priority) const;
signals:

public slots:
private:
    QComboBox *m_comboBox;
};

#endif // PRIORITYWIDGET_H
