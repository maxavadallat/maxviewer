#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QObject>
#include <QFrame>
#include <QImage>

//==============================================================================
// Image Frame Class
//==============================================================================
class ImageFrame : public QFrame
{
    Q_OBJECT

public:
    // Constructor
    explicit ImageFrame(QWidget* aParent = NULL, Qt::WindowFlags aFlags = 0);

    // Load Image
    void loadImage(const QString& aImageFile);

    // Get File Name
    QString getFileName();

    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

    // Destructor
    virtual ~ImageFrame();

signals:

    // Image Updated
    void imageUpdated();

protected: // From QFrame

    // Mosue Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

    // Paint Event
    virtual void paintEvent(QPaintEvent* aEvent);

protected:

    // Image
    QImage      image;

    // Mouse Pressed
    bool        mousePressed;

    // File Name
    QString     fileName;

    // Current Dir
    QString     currentDir;
};

#endif // IMAGEFRAME_H
