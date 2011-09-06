#ifndef KEYASSIGNWIDGET_H
#define KEYASSIGNWIDGET_H

#include <QKeySequence>
#include <QLineEdit>

class KeyAssignWidget : public QLineEdit
{
    Q_OBJECT
public:
    explicit KeyAssignWidget(QWidget *parent = 0);
public:
    void setSequence(QKeySequence seq);
protected:
    virtual bool event(QEvent *event);
signals:
    void assigned(QKeySequence sequence);
public slots:
private:
    QString makeText(bool &final) const;
private:
    bool m_ctrl;
    bool m_alt;
    bool m_shift;
    bool m_meta;
    int m_key;
    QKeySequence m_sequence;
};

#endif // KEYASSIGNWIDGET_H
