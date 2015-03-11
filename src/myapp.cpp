#include <QDebug>

#include "myapp.h"

//==============================================================================
// Constructor
//==============================================================================
MyApplication::MyApplication(int& argc, char**argv)
    : QApplication(argc, argv)
{
    //openFileArgument = QString("Constructor");
}

//==============================================================================
// Get Open File Argument
//==============================================================================
QString MyApplication::getOpenFileArgument()
{
    return openFileArgument;
}

//==============================================================================
// Event Handling
//==============================================================================
bool MyApplication::event(QEvent* event)
{
    // Switch Event Type
    switch (event->type()) {
        // File Open
        case QEvent::FileOpen: {
            // Get File Open Event
            QFileOpenEvent* fileOpenEvent = static_cast<QFileOpenEvent *>(event);
            // Check File Open Event
            if (fileOpenEvent) {

                // Get Open File Argument
                openFileArgument = fileOpenEvent->file();

                qDebug() << "MyApplication::event - openFileArgument: " << openFileArgument;

                // Emit File Open Event Occured
                emit fileOpenEventOccured(openFileArgument);

                return true;
            }
        } break;

        default:

            openFileArgument = QString("someEvent: %1").arg(event->type());
            //qDebug() << "MyApplication::event - type: " << event->type();

        break;
    }

    return QApplication::event(event);
}
