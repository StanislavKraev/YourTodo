#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>
#include <QKeySequence>

#include "actionids.h"

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
    void assigned(QKeySequence sequence);
    void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);
    void onAssign();
    void onReset();
    void stopAssigning();
private:
    void updateState();
    Actions::Actions getCurrentAction() const;
private:
    Ui::PrefsDialog *ui;
    PreferencesModel *m_model;
    IUiManager *m_uiManager;
    int m_selectedRow;
};

#endif // PREFSDIALOG_H
