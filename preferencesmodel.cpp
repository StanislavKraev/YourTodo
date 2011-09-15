#include <QList>

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
    settings.beginGroup("General");
    m_onTop = settings.value("onTop", true).toBool();
    settings.endGroup();
    loadShortcuts(settings);
}

struct ShortHelper
{
    Actions::Actions id;
    QKeySequence seq;
    bool hard;

    ShortHelper(Actions::Actions id, QString seq = QString(), bool hard = false) : id(id), seq(seq), hard(hard) {}
};

void PreferencesModel::loadShortcuts(QSettings &settings)
{
    QList<ShortHelper> shortcuts;

    shortcuts
            << ShortHelper(Actions::FileNew,        "Alt+N")
            << ShortHelper(Actions::FileOpen,       "Alt+O")
            << ShortHelper(Actions::FileSave,       "Ctrl+S")
            << ShortHelper(Actions::FileSaveAs)
            << ShortHelper(Actions::FileMinimize,   "Esc")
            << ShortHelper(Actions::FileExit,       "Alt+F4",       true)

            << ShortHelper(Actions::NewTaskBelow,   "Ctrl+N")
            << ShortHelper(Actions::NewTaskAbove,   "Ctrl+Alt+N")
            << ShortHelper(Actions::NewSubtask,     "Ctrl+Shift+N")

            << ShortHelper(Actions::EditUndo,       "Ctrl+Z",       true)
            << ShortHelper(Actions::EditRedo,       "Ctrl+Shift+Z", true)

            << ShortHelper(Actions::EditCut,        "Ctrl+X",       true)
            << ShortHelper(Actions::EditCopy,       "Ctrl+C",       true)
            << ShortHelper(Actions::EditPaste,      "Ctrl+V",       true)

            << ShortHelper(Actions::EditDeleteSelected, "Del",      true)
            << ShortHelper(Actions::EditSelectAll,  "Ctrl+A",       true)

            << ShortHelper(Actions::ViewMaxTasklist, "Ctrl+M")

            << ShortHelper(Actions::MoveUp,         "Ctrl+Up")
            << ShortHelper(Actions::MoveDown,       "Ctrl+Down")
            << ShortHelper(Actions::MoveRight,      "Ctrl+Right")
            << ShortHelper(Actions::MoveLeft,       "Ctrl+Left")
            << ShortHelper(Actions::MarkAsCompleted,"Ctrl+Shift+Space");

    settings.beginGroup("Shortcuts");
    foreach (ShortHelper h, shortcuts)
        if (h.hard)
            m_hardShortcuts[h.id] = QKeySequence(settings.value(Actions::actionNames[h.id], h.seq).toString());
        else
            m_shortcuts[h.id] = QKeySequence(settings.value(Actions::actionNames[h.id], h.seq).toString());
    m_globalShortcut = QKeySequence(settings.value("GlobalHotKey", "Ctrl+Shift+T").toString());
    settings.endGroup();
}

const QMap<Actions::Actions, QKeySequence> PreferencesModel::shortcuts() const
{
    return m_shortcuts;
}

void PreferencesModel::setShortcut(Actions::Actions action, QKeySequence sequence)
{
    if (m_shortcuts.find(action) != m_shortcuts.end())
    {
        m_shortcuts[action] = sequence;
        QSettings settings;
        settings.beginGroup("Shortcuts");

        QString actionName = Actions::actionNames[action];
        settings.setValue(actionName, sequence.toString(QKeySequence::NativeText));
        settings.endGroup();
        emit(shortcutChanged(action, sequence));
    }
}

QKeySequence PreferencesModel::shortcutForAction(Actions::Actions id) const
{
    if (m_shortcuts.find(id) != m_shortcuts.end())
        return m_shortcuts[id];
    else if (m_hardShortcuts.find(id) != m_hardShortcuts.end())
        return m_hardShortcuts[id];
    return QKeySequence();
}

void PreferencesModel::setGlobalHotkey(QKeySequence sequence)
{
    QSettings settings;
    m_globalShortcut = sequence;
    settings.beginGroup("Shortcuts");
    settings.setValue("GlobalHotKey", m_globalShortcut.toString(QKeySequence::NativeText));
    settings.endGroup();
    emit(globalhotKeyChanged(m_globalShortcut));
}

QKeySequence PreferencesModel::globalShortcut() const
{
    return m_globalShortcut;
}

bool PreferencesModel::onTop() const
{
    return m_onTop;
}

void PreferencesModel::setOnTop(bool onTop)
{
    if (onTop != m_onTop)
    {
        m_onTop = onTop;
        QSettings settings;
        settings.beginGroup("General");
        settings.setValue("onTop", m_onTop);
        settings.endGroup();
        emit(onTopChanged(m_onTop));
    }
}
