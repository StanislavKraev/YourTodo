#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QVariant>

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data) = 0;
    virtual QVariant data() const = 0;
    virtual void clear() = 0;
signals:

public slots:

};

#endif // DATAWIDGET_H
