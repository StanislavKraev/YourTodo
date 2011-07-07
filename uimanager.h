#ifndef UIMANAGER_H
#define UIMANAGER_H

class QMenuBar;

#include <QObject>

class UiManager : public QObject
{
    Q_OBJECT
public:
    UiManager(QMenuBar *menuBar);
public:
    void Init();
public slots:
    void onExit();
private:
    void CreateMenu();
private:
    QMenuBar *m_menuBar;
};

#endif // UIMANAGER_H
