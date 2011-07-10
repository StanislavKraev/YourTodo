#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class MainWindow;

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int &argc, char **argv);
    virtual ~Application();
public:
    MainWindow *mainWindow();
private:
    MainWindow *m_mainWindow;
};

#endif // APPLICATION_H
