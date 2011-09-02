#include "preferencesmodel.h"

PreferencesModel::PreferencesModel()
{
    load();
}

bool PreferencesModel::saveOnMinimize() const
{
    return m_saveOnMinimize;
}

void PreferencesModel::setSaveOnMinimize(bool save)
{
    m_saveOnMinimize = save;
    QSettings settings;
    settings.beginGroup("Autosave");
    settings.setValue("onMinimize", m_saveOnMinimize);
    settings.endGroup();
}

bool PreferencesModel::saveOnExit() const
{
    return m_saveOnExit;
}

void PreferencesModel::setSaveOnExit(bool save)
{
    m_saveOnExit = save;
    QSettings settings;
    settings.beginGroup("Autosave");
    settings.setValue("onExit", m_saveOnExit);
    settings.endGroup();
}

void PreferencesModel::load()
{
    QSettings settings;
    settings.beginGroup("Autosave");
    m_saveOnMinimize = settings.value("onMinimize", true).toBool();
    m_saveOnExit = settings.value("onExit", true).toBool();
    settings.endGroup();
    loadShortcuts(settings);
}

void PreferencesModel::loadShortcuts(QSettings &settings)
{
    settings.beginGroup("Shortcuts");
    m_shortcuts[Actions::FileNew] = QKeySequence(settings.value("FileNew", "Alt+N").toString());
    m_shortcuts[Actions::FileOpen] = QKeySequence(settings.value("FileOpen", "Alt+O").toString());
    m_shortcuts[Actions::FileSave] = QKeySequence(settings.value("FileSave", "Ctrl+S").toString());
    m_shortcuts[Actions::FileSaveAs] = QKeySequence(settings.value("FileSaveAs", "").toString());
    m_shortcuts[Actions::FileMinimize] = QKeySequence(settings.value("FileMinimize", "").toString());
    m_shortcuts[Actions::FileExit] = QKeySequence(settings.value("FileExit", "Ctrl+F4").toString());

    settings.endGroup();
}

const QMap<Actions::Actions, QKeySequence> PreferencesModel::shortcuts() const
{
    return m_shortcuts;
}
