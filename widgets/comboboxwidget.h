#ifndef COMBOBOXWIDGET_H
#define COMBOBOXWIDGET_H

#include "titledwidget.h"

class QComboBox;

class ComboBoxWidget : public TitledWidget
{
    Q_OBJECT
public:
    explicit ComboBoxWidget(QString title, QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data);
    virtual QVariant data() const;
    virtual void clear();
private:
    int priorityFromIndex(int index) const;
    int indexFromPriority(int priority) const;
signals:

public slots:
    void currentIndexChanged(int newIndex);
private:
    QComboBox *m_comboBox;
};

#endif // COMBOBOXWIDGET_H
