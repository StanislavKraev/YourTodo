#ifndef PREFERENCESMODEL_H
#define PREFERENCESMODEL_H

#include <QString>
#include <QKeySequence>
#include <QMap>
#include <QSettings>

#include "actionids.h"

class PreferencesModel
{
public:
    PreferencesModel();
public:
    bool saveOnMinimize() const;
    void setSaveOnMinimize(bool save=true);
    bool saveOnExit() const;
    void setSaveOnExit(bool save=true);
    const QMap<Actions::Actions, QKeySequence> shortcuts() const;
private:
    void load();
    void loadShortcuts(QSettings &settings);
private:
    bool m_saveOnMinimize;
    bool m_saveOnExit;
    QMap<Actions::Actions, QKeySequence> m_shortcuts;
};

#endif // PREFERENCESMODEL_H
