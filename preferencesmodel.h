#ifndef PREFERENCESMODEL_H
#define PREFERENCESMODEL_H

class PreferencesModel
{
public:
    PreferencesModel();
public:
    bool saveOnMinimize() const;
    void setSaveOnMinimize(bool save=true);
    bool saveOnExit() const;
    void setSaveOnExit(bool save=true);
private:
    void load();
private:
    bool m_saveOnMinimize;
    bool m_saveOnExit;
};

#endif // PREFERENCESMODEL_H
