#include <QDebug>

#include "mainbrowserwindow.h"
#include "worker.h"


//==============================================================================
// Constructor
//==============================================================================
Worker::Worker(QObject* aParent)
    : QObject(aParent)
    , mainBrowserWindow(MainBrowserWindow::getInstance())
{
    qDebug() << "Worker::Worker";

    // ...
}

//==============================================================================
// Do Work
//==============================================================================
void Worker::doWork(const int& aOperation)
{
    // Switch Operation
    switch (aOperation) {
        case OTRotateFilesLeft: {
            //qDebug() << "Worker::doWork - Rotate Left";
            // Do Rotate Left
            mainBrowserWindow->doRotateLeft();
        } break;

        case OTRotateFilesRight: {
            //qDebug() << "Worker::doWork - Rotate Right";
            // Do Rotate Right
            mainBrowserWindow->doRotateRigth();
        } break;

        case OTFlipFilesHorizontally: {
            //qDebug() << "Worker::doWork - Flip Horizontally";
            // Do Rotate Right
            mainBrowserWindow->doFlipHorizontally();
        } break;

        case OTFlipFilesVertically: {
            //qDebug() << "Worker::doWork - Flip Vertically";
            // Do Rotate Right
            mainBrowserWindow->doFlipVertically();
        } break;

        case OTDeleteFiles: {
            //qDebug() << "Worker::doWork - Delete";
            // Do Rotate Right
            mainBrowserWindow->doDeleteFile();
        } break;

        case OTRenameFiles: {
            //qDebug() << "Worker::doWork - Rename";
            // Do Rename
            mainBrowserWindow->doRenameFile();
        } break;

        case OTCopyToFiles: {
            //qDebug() << "Worker::doWork - Copy To Dir";
            // Do Rotate Right
            mainBrowserWindow->doCopyToDirectory();
        } break;

        case OTMoveToFiles: {
            //qDebug() << "Worker::doWork - Move To Dir";
            // Do Rotate Right
            mainBrowserWindow->doMoveToDirectory();
        } break;

        case OTFindDuplicates: {
            // Do Find Duplicates
            mainBrowserWindow->doFindDuplicates();
        } break;

        default:
            qDebug() << "Worker::doWork - aOperation: " << aOperation << " - UNHANDLED OPERATION!";
        break;
    }

    // Emit Result Ready
    emit resultReady(aOperation, 0);
}

//==============================================================================
// Stop
//==============================================================================
void Worker::stop()
{
    // Check Worker Thread
    if (mainBrowserWindow->workerThread.isRunning()) {
        qDebug() << "Worker::stop";

        // Terminate Worker Thread
        mainBrowserWindow->workerThread.terminate();

        // ...
    }
}

//==============================================================================
// Destructor
//==============================================================================
Worker::~Worker()
{
    // Stop
    stop();

    qDebug() << "Worker::~Worker";
}

