
#include <QDebug>


#include "myapp.h"
#include "mainbrowserwindow.h"
#include "mainviewerwindow.h"
#include "constants.h"


//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << " Starting Max Viewer...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    // Init Application
    MyApplication app(argc, argv);

    // Set Application Name
    app.setApplicationName(DEFAULT_APPLICATION_NAME);
    // Set Organization Name
    app.setOrganizationName(DEFAULT_ORGANIZATION_NAME);
    // Set Organization Domain
    app.setOrganizationDomain(DEFAULT_ORGANIZATION_DOMAIN);

    // Init Browser Window
    MainBrowserWindow* browserWindow = MainBrowserWindow::getInstance();
    // Init Viewer Window
    MainViewerWindow* viewerWindow = MainViewerWindow::getInstance();

    // Connect Signal
    QObject::connect(&app, SIGNAL(fileOpenEventOccured(QString)), browserWindow, SLOT(handleFileOpenEvent(QString)));

    // Set Viewer Window Visibility
    viewerWindow->setVisible(false);

    // Show Browser Window
    browserWindow->showWindow();

    // Execute App
    int result = app.exec();

    // Release Viewer Window
    viewerWindow->release();

    // Release Browser Window Instance
    browserWindow->release();

    qDebug() << " ";
    qDebug() << "================================================================================";
    qDebug() << " Exiting Max Viewer...";
    qDebug() << "================================================================================";
    qDebug() << " ";

    return result;
}


