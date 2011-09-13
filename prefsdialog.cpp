#include <QKeySequence>
#include "preferencesmodel.h"
#include "iuimanager.h"

#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent, PreferencesModel* model, IUiManager *manager) :
    QDialog(parent),
    ui(new Ui::PrefsDialog),
    m_model(model),
    m_uiManager(manager),
    m_selectedRow(-1),
    m_afterAssign(false)
{
    ui->setupUi(this);

    ui->keysPreviewEdit->setReadOnly(true);
    ui->keysPreviewEdit->setFocusPolicy(Qt::NoFocus);

    ui->gskeysPreviewEdit->setReadOnly(true);
    ui->gskeysPreviewEdit->setFocusPolicy(Qt::NoFocus);

    ui->saveOnExitCheck->setChecked(m_model->saveOnExit());
    ui->saveOnMinimizeCheck->setChecked(m_model->saveOnMinimize());

    connect(ui->saveOnExitCheck, SIGNAL(stateChanged(int)), SLOT(saveOnExitChanged(int)));
    connect(ui->saveOnMinimizeCheck, SIGNAL(stateChanged(int)), SLOT(saveOnMinimizeChanged(int)));

    loadShortcuts();
    connect(ui->keysPreviewEdit, SIGNAL(assigned(QKeySequence)), SLOT(assigned(QKeySequence)));
    connect(ui->keysPreviewEdit, SIGNAL(editingFinished()), SLOT(stopAssigning()));
    connect(ui->shortcutsTableWidget, SIGNAL(currentCellChanged(int,int,int,int)),
                                        SLOT(currentCellChanged(int,int,int,int)));
    connect(ui->assignButton, SIGNAL(clicked()), SLOT(onAssign()));
    connect(ui->resetButton, SIGNAL(clicked()), SLOT(onReset()));
    ui->shortcutsTableWidget->setCurrentCell(0, 0, QItemSelectionModel::ClearAndSelect|QItemSelectionModel::Rows);


    connect(ui->gskeysPreviewEdit, SIGNAL(assigned(QKeySequence)), SLOT(gsAssigned(QKeySequence)));
    connect(ui->gskeysPreviewEdit, SIGNAL(editingFinished()), SLOT(stopGSAssigning()));
    connect(ui->assignGSButton, SIGNAL(clicked()), SLOT(onGSAssign()));
    connect(ui->resetGSButton, SIGNAL(clicked()), SLOT(onGSReset()));
    updateGSState();
}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

void PrefsDialog::saveOnExitChanged(int state)
{
    m_model->setSaveOnExit(state == Qt::Checked);
}

void PrefsDialog::saveOnMinimizeChanged(int state)
{
    m_model->setSaveOnMinimize(state == Qt::Checked);
}

void PrefsDialog::loadShortcuts()
{
    QMap<Actions::Actions, QKeySequence> keys = m_model->shortcuts();
    ui->shortcutsTableWidget->setRowCount(keys.count());
    ui->shortcutsTableWidget->setColumnCount(2);
    ui->shortcutsTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->shortcutsTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    int index = 0;
    foreach(Actions::Actions action, keys.keys())
    {
        QKeySequence seq = keys[action];
        const QAction *rawAction = m_uiManager->action(action);
        if (rawAction)
        {
            QString name = rawAction->text();
            QTableWidgetItem *newItem = new QTableWidgetItem(QString("%1").arg(name));
            ui->shortcutsTableWidget->setItem(index, 0, newItem);
            newItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            QTableWidgetItem *newItem2 = new QTableWidgetItem(QString("%1").arg(seq.toString()));
            newItem2->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
            newItem2->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            ui->shortcutsTableWidget->setItem(index, 1, newItem2);
            ui->shortcutsTableWidget->verticalHeader()->resizeSection(index, 20);
            ++index;
        }
    }
}

void PrefsDialog::currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    m_selectedRow = currentRow;
    updateState();
}

void PrefsDialog::updateState()
{
    ui->keysPreviewEdit->clear();
    int rowCount = m_model->shortcuts().count();
    if (!rowCount || m_selectedRow < 0 || m_selectedRow > rowCount - 1)
        return;

    int rowIndex = 0;
    QMap<Actions::Actions, QKeySequence> keys = m_model->shortcuts();
    foreach(Actions::Actions action, keys.keys())
    {
        if (rowIndex == m_selectedRow)
        {
            ui->keysPreviewEdit->setSequence(keys[action]);
            break;
        }
        ++rowIndex;
    }
}

Actions::Actions PrefsDialog::getCurrentAction() const
{
    int rowCount = m_model->shortcuts().count();
    if (!rowCount || m_selectedRow < 0 || m_selectedRow > rowCount - 1)
        return Actions::Undefined;

    int rowIndex = 0;
    QMap<Actions::Actions, QKeySequence> keys = m_model->shortcuts();
    foreach(Actions::Actions action, keys.keys())
    {
        if (rowIndex == m_selectedRow)
            return action;
        ++rowIndex;
    }
    return Actions::Undefined;
}

void PrefsDialog::assigned(QKeySequence sequence)
{
    Actions::Actions action = getCurrentAction();
    if (action != Actions::Undefined)
    {
        m_model->setShortcut(action, sequence);
        ui->shortcutsTableWidget->item(m_selectedRow, 1)->setData(Qt::DisplayRole, sequence.toString(QKeySequence::NativeText));
    }
    stopAssigning();
}

void PrefsDialog::onAssign()
{
    ui->keysPreviewEdit->setReadOnly(false);
    ui->keysPreviewEdit->setFocusPolicy(Qt::TabFocus);
    ui->keysPreviewEdit->setFocus();
    ui->keysPreviewEdit->activateWindow();
    ui->keysPreviewEdit->clear();
}

void PrefsDialog::stopAssigning()
{
    ui->keysPreviewEdit->setReadOnly(true);
    ui->keysPreviewEdit->clearFocus();
    ui->keysPreviewEdit->setFocusPolicy(Qt::NoFocus);
    updateState();
}

void PrefsDialog::onReset()
{
    stopAssigning();
    ui->keysPreviewEdit->clear();
    Actions::Actions action = getCurrentAction();
    if (action != Actions::Undefined)
    {
        m_model->setShortcut(action, QKeySequence());
        ui->shortcutsTableWidget->item(m_selectedRow, 1)->setData(Qt::DisplayRole, "");
    }
}

void PrefsDialog::gsAssigned(QKeySequence sequence)
{
    m_afterAssign = true;
    m_storedGSK = QKeySequence();
    m_model->setGlobalHotkey(sequence);

    ui->gskeysPreviewEdit->setReadOnly(true);
    ui->gskeysPreviewEdit->clearFocus();
    ui->gskeysPreviewEdit->setFocusPolicy(Qt::NoFocus);
    updateGSState();
}

void PrefsDialog::onGSAssign()
{
    m_storedGSK = m_model->globalShortcut();
    m_model->setGlobalHotkey(QKeySequence());
    ui->gskeysPreviewEdit->clear();
    ui->gskeysPreviewEdit->setReadOnly(false);
    ui->gskeysPreviewEdit->setFocusPolicy(Qt::TabFocus);
    ui->gskeysPreviewEdit->setFocus();
    ui->gskeysPreviewEdit->activateWindow();
}

void PrefsDialog::stopGSAssigning()
{
    if (!m_afterAssign)
    {
        m_model->setGlobalHotkey(m_storedGSK);
        ui->gskeysPreviewEdit->setReadOnly(true);
        ui->gskeysPreviewEdit->clearFocus();
        ui->gskeysPreviewEdit->setFocusPolicy(Qt::NoFocus);
        updateGSState();
    }
    m_afterAssign = false;
}

void PrefsDialog::onGSReset()
{
    stopGSAssigning();
    ui->gskeysPreviewEdit->clear();
    m_model->setGlobalHotkey(QKeySequence());
}

void PrefsDialog::updateGSState()
{
    ui->gskeysPreviewEdit->clear();
    ui->gskeysPreviewEdit->setSequence(m_model->globalShortcut());
}
