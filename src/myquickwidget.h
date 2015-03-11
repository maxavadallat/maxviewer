#ifndef MYQUICKWIDGET_H
#define MYQUICKWIDGET_H

#include <QQuickWidget>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QPoint>

class MainBrowserWindow;


//==============================================================================
// My Quick Widget Class
//==============================================================================
class MyQuickWidget : public QQuickWidget
{
    Q_OBJECT

public:

    // Constructor
    explicit MyQuickWidget(QWidget* aParent = NULL);

    // Destructor
    virtual ~MyQuickWidget();

signals:

    // Focus Changed Signal
    void focuseChanged(const bool& aFocused);
    // Mouse Pressed Signal
    void mousePressed(const QPoint& aPos, const Qt::MouseButton& aButton);
    // Mouse Released Signal
    void mouseReleased(const QPoint& aPos, const Qt::MouseButton& aButton);

protected: // from QQuickWidget

    // Focus In Event
    virtual void focusInEvent(QFocusEvent* aEvent);
    // Focus Out Event
    virtual void focusOutEvent(QFocusEvent* aEvent);

    // Mouse Press Event
    virtual void mousePressEvent(QMouseEvent* aEvent);
    // Mouse Release Event
    virtual void mouseReleaseEvent(QMouseEvent* aEvent);

protected:

    // Focused
    bool       focused;
};

#endif // MYQUICKWIDGET_H
