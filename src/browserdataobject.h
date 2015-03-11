#ifndef BROWSERDATAOBJECT_H
#define BROWSERDATAOBJECT_H

#include <QObject>
#include <QString>

enum BDOFType
{
    FTFile  = 0,
    FTDir
};


//==============================================================================
// Browser Model Data Object
//==============================================================================
class BrowserDataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(int fileType READ getFileType WRITE setFileType NOTIFY fileTypeChanged)
    Q_PROPERTY(bool fileSelected READ getFileSelected WRITE setFileSelected NOTIFY fileSelectedChanged)


public:

    // Constructor
    BrowserDataObject(QObject* aParent = NULL);

    // Constructor
    explicit BrowserDataObject(const QString& aFileName, const BDOFType& aType = FTFile, QObject* aParent = NULL);

    // Set File Name
    void setFileName(const QString& aFileName);
    // Get File Name
    QString getFileName();

    // Set File Type
    void setFileType(const int& aFileType);
    // Get File Type
    int getFileType();

    // Set File Selected
    void setFileSelected(const bool& aSelected);
    // Get File Selected
    bool getFileSelected();

    // Destructor
    virtual ~BrowserDataObject();

signals:

    // File Name Changed Signal
    void fileNameChanged(const QString& aFileName);

    // File Type Changed Signal
    void fileTypeChanged(const int& aFileType);

    // File Selected Changed Signal
    void fileSelectedChanged(const bool& aSelected);

private:
    friend class MainBrowserWindow;

    // File Name
    QString     fileName;

    // File Type
    BDOFType    fileType;

    // File selected
    bool        fileSelected;
};

#endif // BROWSERDATAOBJECT_H
