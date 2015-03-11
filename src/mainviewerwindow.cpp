#include <QDebug>
#include <QApplication>
#include <QSettings>
#include <QQmlContext>
#include <QRect>

#include "mainbrowserwindow.h"
#include "mainviewerwindow.h"
#include "constants.h"

#include "ui_mainviewerwindow.h"

// Main Viewer Window Singleton
static MainViewerWindow* mainViewerWindow = NULL;

//==============================================================================
// Static Constructor
//==============================================================================
MainViewerWindow* MainViewerWindow::getInstance()
{
    // Check Singleton
    if (!mainViewerWindow) {
        // Create Main Browser Window
        mainViewerWindow = new MainViewerWindow();
    }

    return mainViewerWindow;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainViewerWindow::release()
{
    // Delete Main Browser Window
    delete mainViewerWindow;
    // Reset Singleton
    mainViewerWindow = NULL;
}

//==============================================================================
// Constructor
//==============================================================================
MainViewerWindow::MainViewerWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainViewerWindow)
    , mainBrowserWindow(MainBrowserWindow::getInstance())
    , thumbsVisible(false)
{
    // Setup UI
    ui->setupUi(this);

    // Get QML Context
    QQmlContext* qmlContext = ui->viewerWidget->rootContext();

    // Set Context Property - Main View Controller
    qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, mainBrowserWindow);

    // Set Context Property
    qmlContext->setContextProperty(VIEWER_VIEW_CONTROLLER, this);

    // Set Source
    ui->viewerWidget->setSource(QUrl("qrc:/qml/viewer.qml"));

    // Restore UI
    restoreUI();
}

//==============================================================================
// Restore UI
//==============================================================================
void MainViewerWindow::restoreUI()
{
    qDebug() << "MainViewerWindow::restoreUI";

    // Init Settings
    QSettings settings;

    // Set Width & Height
    setGeometry(QRect(settings.value(SETTINGS_KEY_VIEWER_POSX).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_POSY).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_WIDTH).toInt(),
                      settings.value(SETTINGS_KEY_VIEWER_HEIGHT).toInt()));

    // Connect Signals
    //connect(mainBrowserWindow, SIGNAL(currentFileChanged(QString)), this, SLOT(currentFileChanged(QString)));
}

//==============================================================================
// Save Settings
//==============================================================================
void MainViewerWindow::saveSettings()
{
    qDebug() << "MainViewerWindow::saveSettings";

    // ...

    // Init Settings
    QSettings settings;

    // Get Full Screen
    bool fullScreen = windowState() == Qt::WindowFullScreen;

    // Save Full Screen State
    settings.setValue(SETTINGS_KEY_VIEWER_FULLSCREEN, fullScreen);

    // Check Full Screen
    if (!fullScreen) {
        // Get Geometry
        QRect currRect = geometry();

        // Save Window Position
        settings.setValue(SETTINGS_KEY_VIEWER_POSX, currRect.x());
        settings.setValue(SETTINGS_KEY_VIEWER_POSY, currRect.y());
        // Save Window Width & Height
        settings.setValue(SETTINGS_KEY_VIEWER_WIDTH, currRect.width());
        settings.setValue(SETTINGS_KEY_VIEWER_HEIGHT, currRect.height());
    }
}

//==============================================================================
// Set Thumbs Visible
//==============================================================================
void MainViewerWindow::setThumbsVisible(const bool& aVisible)
{
    // Check Thumbs Visible
    if (thumbsVisible != aVisible) {
        // Set Thumbs Visible
        thumbsVisible = aVisible;

        // Emit Thumbs Visible Changed Signal
        emit thumbsVisibleChanged(thumbsVisible);
    }
}

//==============================================================================
// Get Thumbs Visible
//==============================================================================
bool MainViewerWindow::getThumbsVisible()
{
    return thumbsVisible;
}

//==============================================================================
// Launch Browser Window
//==============================================================================
void MainViewerWindow::launchBrowserWindow()
{
    qDebug() << "MainViewerWindow::launchBrowserWindow";

    // Hide
    hide();

    // Show Main Browser Window
    mainBrowserWindow->raise();
}

//==============================================================================
// Launc Preferences
//==============================================================================
void MainViewerWindow::launcPreferences()
{
    qDebug() << "MainViewerWindow::launchBrowserWindow";

    // Launch Preferences
    mainBrowserWindow->launchPreferences();
}

//==============================================================================
// Show Window
//==============================================================================
void MainViewerWindow::showWindow()
{
    qDebug() << "MainViewerWindow::showWindow";

    // Init Settings
    QSettings settings;

    // Check Full Screen
    if (settings.value(SETTINGS_KEY_VIEWER_FULLSCREEN).toBool()) {
        // Show Full Screen
        showFullScreen();
    } else {
        // Hide
        hide();

        // Set Width & Height
        setGeometry(QRect(settings.value(SETTINGS_KEY_VIEWER_POSX).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_POSY).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_WIDTH).toInt(),
                          settings.value(SETTINGS_KEY_VIEWER_HEIGHT).toInt()));

        // Show
        showNormal();
        // Raise
        raise();
    }
}

//==============================================================================
// Hide Window
//==============================================================================
void MainViewerWindow::hideWindow()
{
    qDebug() << "MainViewerWindow::hideWindow";

    // Save Settings
    saveSettings();

    // hide
    hide();
}

//==============================================================================
// Show Status Text
//==============================================================================
void MainViewerWindow::showStatusText(const QString& aStatusText, const int& aTimeout)
{
    // Show Message
    ui->statusbar->showMessage(aStatusText, aTimeout);
}

//==============================================================================
// Current File Changed Slot
//==============================================================================
void MainViewerWindow::currentFileChanged(const QString& aFilePath)
{
    // Show Status Text
    showStatusText(aFilePath, 5000);
}

//==============================================================================
// Get QML Context
//==============================================================================
QQmlContext* MainViewerWindow::getQMLContext()
{
    return ui->viewerWidget->rootContext();
}

//==============================================================================
// On Action About Triggered
//==============================================================================
void MainViewerWindow::on_actionAbout_triggered()
{
    // Launch About Dialog
}

//==============================================================================
// On Action Preferences Triggered
//==============================================================================
void MainViewerWindow::on_actionPreferences_triggered()
{
    // Launch Preferences
    launcPreferences();
}

//==============================================================================
// On Action Rename Triggered
//==============================================================================
void MainViewerWindow::on_actionRename_triggered()
{
    // Rename Current File
    mainBrowserWindow->renameFile();
}

//==============================================================================
// On Action Delete Triggered
//==============================================================================
void MainViewerWindow::on_actionDelete_triggered()
{
    // Delete Current File
    mainBrowserWindow->deleteFile();
}

//==============================================================================
// On Action Rotate Left Triggered
//==============================================================================
void MainViewerWindow::on_actionRotate_Left_triggered()
{
    // Rotate Left Current File
    mainBrowserWindow->rotateLeft();
}

//==============================================================================
// On Action Rotate Right Triggered
//==============================================================================
void MainViewerWindow::on_actionRotate_Right_triggered()
{
    // Rotatet Right Current File
    mainBrowserWindow->rotateRigth();
}

//==============================================================================
// On Action Flip Horizontally Trigered
//==============================================================================
void MainViewerWindow::on_actionFlip_Horizontally_triggered()
{
    // Flip Current File Horizontally
    mainBrowserWindow->flipHorizontally();
}

//==============================================================================
// On Action Flip Vertically Triggered
//==============================================================================
void MainViewerWindow::on_actionFlip_Vertically_triggered()
{
    // Flip Current File Vertically
    mainBrowserWindow->flipVertically();
}

//==============================================================================
// On Action Slide Show Triggered
//==============================================================================
void MainViewerWindow::on_actionSlide_Show_triggered()
{
    // Toggle Slide Show
    mainBrowserWindow->toggleSlideShow();
}

//==============================================================================
// On Action Quit Triggered
//==============================================================================
void MainViewerWindow::on_actionQuit_triggered()
{
    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Destructor
//==============================================================================
MainViewerWindow::~MainViewerWindow()
{
    // Save Settings
    //saveSettings();

    // Delete Viewer Widget to Avoid Crash
    delete ui->viewerWidget;

    // De;ete UI
    delete ui;

    qDebug() << "MainViewerWindow::~MainViewerWindow";
}

