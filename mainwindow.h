#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class TreeUi;
class UiManager;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    void SetupEventFilter();
    void onShortcut();
protected:
    virtual void changeEvent(QEvent *event);

private:
    Ui::MainWindow *ui;
    TreeUi *m_treeUi;
    UiManager *m_uiManager;
};

#endif // MAINWINDOW_H
