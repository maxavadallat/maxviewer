#include <QDebug>

#include "myquickwidget.h"

//==============================================================================
// Constructor
//==============================================================================
MyQuickWidget::MyQuickWidget(QWidget* aParent)
    : QQuickWidget(aParent)
    , focused(false)
{
    //qDebug() << "MyQuickWidget::MyQuickWidget";

    // ...
}

//==============================================================================
// Focus In Event
//==============================================================================
void MyQuickWidget::focusInEvent(QFocusEvent* aEvent)
{
    QQuickWidget::focusInEvent(aEvent);

    //qDebug() << "MyQuickWidget::focusInEvent";

    // Check Focused
    if (!focused) {
        // Set Focused
        focused = true;

        // Emit Focus Changed Signal
        emit focuseChanged(focused);
    }
}

//==============================================================================
// Focus Out Event
//==============================================================================
void MyQuickWidget::focusOutEvent(QFocusEvent* aEvent)
{
    QQuickWidget::focusOutEvent(aEvent);

    //qDebug() << "MyQuickWidget::focusOutEvent";

    // Check Focused
    if (focused) {
        // Set Focused
        focused = false;

        // Emit Focus Changed Signal
        emit focuseChanged(focused);
    }
}

//==============================================================================
// Mouse Press Event
//==============================================================================
void MyQuickWidget::mousePressEvent(QMouseEvent* aEvent)
{
    QQuickWidget::mousePressEvent(aEvent);

    // Check Event
    if (aEvent) {
        //qDebug() << "MyQuickWidget::mousePressEvent - pos: " << aEvent->pos() << " - button: " << aEvent->button();

        // Emit Mouse Pressed Signal
        emit mousePressed(aEvent->pos(), aEvent->button());
    }
}

//==============================================================================
// Mouse Release Event
//==============================================================================
void MyQuickWidget::mouseReleaseEvent(QMouseEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        //qDebug() << "MyQuickWidget::mouseReleaseEvent - pos: " << aEvent->pos() << " - button: " << aEvent->button();

        // Emit Mouse Released Signal
        emit mouseReleased(aEvent->pos(), aEvent->button());
    }

    QQuickWidget::mouseReleaseEvent(aEvent);
}

//==============================================================================
// Destructor
//==============================================================================
MyQuickWidget::~MyQuickWidget()
{
    // ...

    //qDebug() << "MyQuickWidget::~MyQuickWidget";
}

