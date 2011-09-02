#include <QSettings>

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
}
