#ifndef PREFERENCESMODEL_H
#define PREFERENCESMODEL_H

#include <QString>
#include <QKeySequence>
#include <QMap>
#include <QSettings>

#include "actionids.h"

class PreferencesModel : public QObject
{
    Q_OBJECT
public:
    PreferencesModel();
public:
    bool saveOnMinimize() const;
    void setSaveOnMinimize(bool save=true);
    bool saveOnExit() const;
    void setSaveOnExit(bool save=true);
    const QMap<Actions::Actions, QKeySequence> shortcuts() const;
    void setShortcut(Actions::Actions action, QKeySequence sequence);
    void setGlobalHotkey(QKeySequence sequence);
    QKeySequence shortcutForAction(Actions::Actions id) const;
    QKeySequence globalShortcut() const;
    bool onTop() const;
    void setOnTop(bool onTop);
signals:
    void shortcutChanged(Actions::Actions action, QKeySequence sequence);
    void globalhotKeyChanged(QKeySequence sequence);
    void onTopChanged(bool newState);
private:
    void load();
    void loadShortcuts(QSettings &settings);
private:
    bool m_saveOnMinimize;
    bool m_saveOnExit;
    bool m_onTop;
    QMap<Actions::Actions, QKeySequence> m_shortcuts;
    QMap<Actions::Actions, QKeySequence> m_hardShortcuts;
    QKeySequence m_globalShortcut;
};

#endif // PREFERENCESMODEL_H
