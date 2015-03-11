#include <QPainter>
#include <QDebug>

#include "imagecompareframe.h"
#include "utility.h"

//==============================================================================
// Constructor
//==============================================================================
ImageCompareFrame::ImageCompareFrame(QWidget* aParent, Qt::WindowFlags aFlags)
    : QFrame(aParent, aFlags)
    , identical(false)
    , mousePressed(false)
{
    //qDebug() << "ImageCompareFrame::ImageCompareFrame";

    // ...
}

//==============================================================================
// Set Left Image
//==============================================================================
void ImageCompareFrame::setLeftImage(const QImage& aImage)
{
    // Set Left Image
    leftImage = aImage;

    // Compare Images
    compareImages();

    // Update
    update();
}

//==============================================================================
// Set Left Image
//==============================================================================
void ImageCompareFrame::setLeftImage(const QString& aImageFile)
{
    // Set Left Image
    leftImage = QImage(aImageFile);

    // Compare Images
    compareImages();

    // Update
    update();
}

//==============================================================================
// Set Right Image
//==============================================================================
void ImageCompareFrame::setRightImage(const QImage& aImage)
{
    // Set Right Image
    rightImage = aImage;

    // Compare Images
    compareImages();

    // Update
    update();
}

//==============================================================================
// Set Right Image
//==============================================================================
void ImageCompareFrame::setRightImage(const QString& aImageFile)
{
    // Set Right Image
    rightImage = QImage(aImageFile);

    // Compare Images
    compareImages();

    // Update
    update();
}

//==============================================================================
// Compare Images
//==============================================================================
void ImageCompareFrame::compareImages()
{
    // Compare Images
    identical = compareImagesByPixel(leftImage, rightImage);

    // ...
}

//==============================================================================
// Is Identical
//==============================================================================
bool ImageCompareFrame::isIdentical()
{
    return identical;
}

//==============================================================================
// Mosue Press Event
//==============================================================================
void ImageCompareFrame::mousePressEvent(QMouseEvent* aEvent)
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
void ImageCompareFrame::mouseReleaseEvent(QMouseEvent* aEvent)
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

            // Compare Images
            compareImages();

            // ...
        }
    }
}

//==============================================================================
// Paint Event
//==============================================================================
void ImageCompareFrame::paintEvent(QPaintEvent* aEvent)
{
    QFrame::paintEvent(aEvent);

    // Check Event
    if (aEvent) {
        // Init Painter
        QPainter p(this);
        // Init Overlay Draw Width
        int drawWidth = 0;
        // Init Overlay Draw Height
        int drawHeight = 0;

        // Check Left Image
        if (!leftImage.isNull()) {
            // Get Scaled Left Image
            QImage scaledLeftImage = leftImage.scaled(width(), height(), Qt::KeepAspectRatio, Qt::FastTransformation);
            // Check Scaled Image
            if (!scaledLeftImage.isNull()) {
                // Set Overlay Draw Width
                drawWidth = scaledLeftImage.width();
                // Set Overlay Draw Height
                drawHeight = scaledLeftImage.height();
                // Draw Left Image
                p.drawImage((width() - scaledLeftImage.width()) >> 1, (height() - scaledLeftImage.height()) >> 1, scaledLeftImage);
            }
        }

        // Check Right Image
        if (!rightImage.isNull()) {
            // Get Scaled Right Image
            QImage scaledRightImage = rightImage.scaled(width(), height(), Qt::KeepAspectRatio, Qt::FastTransformation);

            // Check Scaled Image
            if (!scaledRightImage.isNull()) {
                // Set Opacity
                p.setOpacity(0.7);
                // Draw Right Image
                p.drawImage((width() - scaledRightImage.width()) >> 1, (height() - scaledRightImage.height()) >> 1, scaledRightImage);
            }
        }

        // Set Pen
        p.setPen(Qt::NoPen);
        // Set Brush
        p.setBrush(identical ? QColor(0, 128, 0, 128) : QColor(128, 0, 0, 128));
        // Draw Overlay Rect
        p.drawRect(QRect((width() - drawWidth) >> 1, (height() - drawHeight) >> 1, drawWidth, drawHeight));

        //qDebug() << "[" << width() << "x" << height() << "]";
    }
}

//==============================================================================
// Destructor
//==============================================================================
ImageCompareFrame::~ImageCompareFrame()
{
    // ...

    //qDebug() << "ImageCompareFrame::~ImageCompareFrame";
}

