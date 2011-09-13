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
    void currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void assigned(QKeySequence sequence);
    void onAssign();
    void onReset();
    void stopAssigning();

    void gsAssigned(QKeySequence sequence);
    void onGSAssign();
    void onGSReset();
    void stopGSAssigning();
private:
    void updateState();
    void updateGSState();
    Actions::Actions getCurrentAction() const;
private:
    Ui::PrefsDialog *ui;
    PreferencesModel *m_model;
    IUiManager *m_uiManager;
    int m_selectedRow;
    QKeySequence m_storedGSK;
    bool m_afterAssign;
};

#endif // PREFSDIALOG_H
