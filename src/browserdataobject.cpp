#include <QDebug>

#include "browserdataobject.h"

//==============================================================================
// Constructor
//==============================================================================
BrowserDataObject::BrowserDataObject(QObject* aParent)
    : QObject(aParent)
{
}

//==============================================================================
// Constructor
//==============================================================================
BrowserDataObject::BrowserDataObject(const QString& aFileName, const BDOFType& aType, QObject* aParent)
    : QObject(aParent)
    , fileName(aFileName)
    , fileType(aType)
    , fileSelected(false)
{
    //qDebug() << "BrowserDataObject::BrowserDataObject";

    // ...
}

//==============================================================================
// Set File Name
//==============================================================================
void BrowserDataObject::setFileName(const QString& aFileName)
{
    // Check File Name
    if (fileName != aFileName) {
        //qDebug() << "BrowserDataObject::setFileName - aFileName: " << aFileName;
        // Set File Name
        fileName = aFileName;

        // Emit File Name Changed Signal
        emit fileNameChanged(fileName);
    }
}

//==============================================================================
// Get File Name
//==============================================================================
QString BrowserDataObject::getFileName()
{
    return fileName;
}

//==============================================================================
// Set File Type
//==============================================================================
void BrowserDataObject::setFileType(const int& aFileType)
{
    // Check File Type
    if (fileType != aFileType) {
        //qDebug() << "BrowserDataObject::setFileType - aFileType: " << aFileType;
        // Set File Type
        fileType = (BDOFType)aFileType;

        // Emit File Type Changed Signal
        emit fileTypeChanged(fileType);
    }
}

//==============================================================================
// Get File Type
//==============================================================================
int BrowserDataObject::getFileType()
{
    return (int)fileType;
}

//==============================================================================
// Set File Selected
//==============================================================================
void BrowserDataObject::setFileSelected(const bool& aSelected)
{
    // Check File Selected
    if (fileSelected != aSelected) {
        //qDebug() << "BrowserDataObject::setFileSelected - aSelected: " << aSelected;
        // Set Selected
        fileSelected = aSelected;

        // Emit Fiel Selected Changed Signal
        emit fileSelectedChanged(fileSelected);
    }
}

//==============================================================================
// Get File Selected
//==============================================================================
bool BrowserDataObject::getFileSelected()
{
    return fileSelected;
}

//==============================================================================
// Destructor
//==============================================================================
BrowserDataObject::~BrowserDataObject()
{
    // ...

    //qDebug() << "BrowserDataObject::~BrowserDataObject";
}

