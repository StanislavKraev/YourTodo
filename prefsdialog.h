#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>

namespace Ui {
    class PrefsDialog;
}

class PreferencesModel;
class IUiManager;

class PrefsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PrefsDialog(QWidget *parent, PreferencesModel *model, IUiManager *manager);
    ~PrefsDialog();
public:
    void loadShortcuts();
private slots:
    void saveOnExitChanged(int state);
    void saveOnMinimizeChanged(int state);
private:
    Ui::PrefsDialog *ui;
    PreferencesModel *m_model;
    IUiManager *m_uiManager;
};

#endif // PREFSDIALOG_H
