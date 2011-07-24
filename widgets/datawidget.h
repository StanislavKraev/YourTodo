#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QVariant>

class DataWidgetModel;

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(QWidget *parent = 0);
public:
    virtual void setData(const QVariant &data) = 0;
    virtual QVariant data() const = 0;
    virtual void clear() = 0;
public:
    void setModel(DataWidgetModel *model);
    DataWidgetModel *model();
protected:
    void onDataChanged(QVariant data);
signals:

public slots:
    void dataChanged();
private:
    DataWidgetModel *m_model;
};

#endif // DATAWIDGET_H
