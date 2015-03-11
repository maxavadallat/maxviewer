#ifndef IMAGECOMPAREFRAME_H
#define IMAGECOMPAREFRAME_H

#include <QFrame>
#include <QImage>
#include <QPaintEvent>

//==============================================================================
// Image Compare Frame Class
//==============================================================================
class ImageCompareFrame : public QFrame
{
    Q_OBJECT

public:
    // Constructor
    explicit ImageCompareFrame(QWidget* aParent = NULL, Qt::WindowFlags aFlags = 0);

    // Set Left Image
    void setLeftImage(const QImage& aImage);
    // Set Left Image
    void setLeftImage(const QString& aImageFile);

    // Set Right Image
    void setRightImage(const QImage& aImage);
    // Set Right Image
    void setRightImage(const QString& aImageFile);

    // Compare Images
    void compareImages();

    // Is Identical
    bool isIdentical();

    // Destructor
    virtual ~ImageCompareFrame();

protected: // From QFrame

    // Mosue Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    // Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:

    // Left Image
    QImage      leftImage;

    // Right Image
    QImage      rightImage;

    // Images Identical
    bool        identical;

    // Mouse Pressed
    bool        mousePressed;

};

#endif // IMAGECOMPAREFRAME_H
