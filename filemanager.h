#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>

#include "itool.h"

class FileManager : public QObject, public ITool
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);

signals:

public slots:
    void onNew();
    void onOpen();
    void onSave();
    void onSaveAs();
    void onClose();
};

#endif // FILEMANAGER_H
