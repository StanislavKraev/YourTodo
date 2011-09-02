#ifndef KEYASSIGNWIDGET_H
#define KEYASSIGNWIDGET_H

#include <QLineEdit>

class KeyAssignWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit KeyAssignWidget(QWidget *parent = 0);
protected:
    virtual bool event(QEvent *event);
signals:

public slots:
private:
    bool m_ctrl;
    bool m_alt;
    int m_key;
};

#endif // KEYASSIGNWIDGET_H
