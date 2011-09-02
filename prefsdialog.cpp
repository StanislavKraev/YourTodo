#include "preferencesmodel.h"

#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent, PreferencesModel* model) :
    QDialog(parent),
    ui(new Ui::PrefsDialog),
    m_model(model)
{
    ui->setupUi(this);

    ui->saveOnExitCheck->setChecked(m_model->saveOnExit());
    ui->saveOnMinimizeCheck->setChecked(m_model->saveOnMinimize());

    connect(ui->saveOnExitCheck, SIGNAL(stateChanged(int)), SLOT(saveOnExitChanged(int)));
    connect(ui->saveOnMinimizeCheck, SIGNAL(stateChanged(int)), SLOT(saveOnMinimizeChanged(int)));
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
