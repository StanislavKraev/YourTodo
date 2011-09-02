#ifndef IFILEMANAGER_H
#define IFILEMANAGER_H

class IFileManager
{
protected:
    IFileManager() {}
    ~IFileManager() {}
private:
    IFileManager(const IFileManager&);
    IFileManager& operator=(const IFileManager&);
public:
    virtual void autoSave() = 0;
};

#endif // IFILEMANAGER_H
