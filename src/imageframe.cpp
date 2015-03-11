
#include <QFile>
#include <QDir>
#include <QPainter>
#include <QFileDialog>
#include <QStringList>
#include <QDebug>

#include "imageframe.h"


//==============================================================================
// Constructor
//==============================================================================
ImageFrame::ImageFrame(QWidget* aParent, Qt::WindowFlags aFlags)
    : QFrame(aParent, aFlags)
    , mousePressed(false)
    , currentDir(QDir::homePath())
{
    //qDebug() << "ImageFrame::ImageFrame";

    // Set Shape
    setFrameShape(QFrame::StyledPanel);
    // Set Shadow
    setFrameShadow(QFrame::Plain);
}

//==============================================================================
// Load Image
//==============================================================================
void ImageFrame::loadImage(const QString& aImageFile)
{
    // Check If File Exists
    if (QFile::exists(aImageFile)) {
        // Set File Name
        fileName = aImageFile;
        // Set Image
        image = QImage(aImageFile);

        // Emit Image Updated Signal
        emit imageUpdated();
    }
}

//==============================================================================
// Get File Name
//==============================================================================
QString ImageFrame::getFileName()
{
    return fileName;
}

//==============================================================================
// Set Current Dir
//==============================================================================
void ImageFrame::setCurrentDir(const QString& aCurrentDir)
{
    // Set Current Dir
    currentDir = aCurrentDir;
}

//==============================================================================
// Paint Event
//==============================================================================
void ImageFrame::paintEvent(QPaintEvent* aEvent)
{
    QFrame::paintEvent(aEvent);

    // Check Event
    if (aEvent && !image.isNull()) {
        // Init Painter
        QPainter p(this);
        // Get Scaled Image
        QImage scaledImage = image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::FastTransformation);
        // Check Scaled Image
        if (!scaledImage.isNull()) {
            // Draw Scaled Image
            p.drawImage((width() - scaledImage.width()) >> 1, (height() - scaledImage.height()) >> 1, scaledImage);
        }
    }
}

//==============================================================================
// Mosue Press Event
//==============================================================================
void ImageFrame::mousePressEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Set Mouse Pressed
        mousePressed = true;

        // Set Shadow
        setFrameShadow(QFrame::Sunken);

        // Update
        update();

        // ...
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void ImageFrame::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Mosue Pressed
        if (mousePressed) {
            // reset Mouse Pressed
            mousePressed = false;
            // Set Shadow
            setFrameShadow(QFrame::Plain);

            // Update
            update();

            // Launch File Dialog
            QFileDialog fileDialog;
            // Configure File Dialog
            //fileDialog.setDirectory(QDir::homePath());
            fileDialog.setDirectory(currentDir);

            // Init Filters
            QStringList filters;
            // Append Filters
            filters << "Image files (*.jpg *.jpeg *.png *.gif *.bmp)"
                    << "Any files (*)";

            // Set Name Filters
            fileDialog.setNameFilters(filters);

            // Exec File Dialog
            if (fileDialog.exec()) {
                // Load Image
                loadImage(fileDialog.selectedFiles().at(0));
                // Update
                update();
            }
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
ImageFrame::~ImageFrame()
{
    // ...

    //qDebug() << "ImageFrame::~ImageFrame";
}

