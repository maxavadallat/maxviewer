//#include <QApplication>
//#include <QEvent>
//#include <QFileOpenEvent>
//#include <QFile>
//#include <QString>
#include <QDebug>

#include "myapp.h"
#include "mainbrowserwindow.h"
#include "mainviewerwindow.h"
#include "constants.h"

// Open File Argument
//static QString openFileArgument = QString("");



//==============================================================================
// Main
//==============================================================================
int main(int argc, char* argv[])
{
    // Init Application
    //QApplication app(argc, argv);
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

    // ...

    // Execute App
    int result = app.exec();

    // Release Viewer Window
    viewerWindow->release();

    // Release Browser Window Instance
    browserWindow->release();

    return result;
}


/*

defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>jpg</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>jpeg</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>gif</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>png</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>bmp</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"

/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister -f /Applications/MaxViewer.app/

/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister -kill -domain local -domain system -domain user

*/
