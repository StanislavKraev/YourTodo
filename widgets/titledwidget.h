#ifndef TITLEDWIDGET_H
#define TITLEDWIDGET_H

#include "datawidget.h"
class QLabel;
class QHBoxLayout;

class TitledWidget : public DataWidget
{
    Q_OBJECT
public:
    explicit TitledWidget(QString title, QWidget *parent = 0);

signals:

public slots:
private:
    QLabel *m_label;
    QHBoxLayout *m_layout;
};

#endif // TITLEDWIDGET_H
