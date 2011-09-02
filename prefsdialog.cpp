#include <QKeySequence>
#include "preferencesmodel.h"
#include "iuimanager.h"

#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent, PreferencesModel* model, IUiManager *manager) :
    QDialog(parent),
    ui(new Ui::PrefsDialog),
    m_model(model),
    m_uiManager(manager)
{
    ui->setupUi(this);

    ui->saveOnExitCheck->setChecked(m_model->saveOnExit());
    ui->saveOnMinimizeCheck->setChecked(m_model->saveOnMinimize());

    connect(ui->saveOnExitCheck, SIGNAL(stateChanged(int)), SLOT(saveOnExitChanged(int)));
    connect(ui->saveOnMinimizeCheck, SIGNAL(stateChanged(int)), SLOT(saveOnMinimizeChanged(int)));

    loadShortcuts();
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
            QTableWidgetItem *newItem2 = new QTableWidgetItem(QString("%1").arg(seq.toString()));
            newItem2->setData(Qt::TextAlignmentRole, Qt::AlignCenter);
            ui->shortcutsTableWidget->setItem(index, 1, newItem2);
            ui->shortcutsTableWidget->verticalHeader()->resizeSection(index, 20);
            ++index;
        }
    }
}
