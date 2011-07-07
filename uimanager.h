#ifndef UIMANAGER_H
#define UIMANAGER_H

class QMenuBar;
class QWidget;

#include <QObject>

class UiManager : public QObject
{
    Q_OBJECT
public:
    UiManager(QWidget *parent, QMenuBar *menuBar);
public:
    void Init();
public slots:
    void onExit();
    void onNewTasklist();
private:
    void CreateMenu();
private:
    QMenuBar *m_menuBar;
    QWidget *m_parentWindow;
};

#endif // UIMANAGER_H
