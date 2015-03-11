#ifndef MAINVIEWERWINDOW_H
#define MAINVIEWERWINDOW_H

#include <QMainWindow>
#include <QQmlContext>

namespace Ui {
class MainViewerWindow;
}

class MainBrowserWindow;


//==============================================================================
// Main Viewer Window Class
//==============================================================================
class MainViewerWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(bool thumbsVisible READ getThumbsVisible WRITE setThumbsVisible NOTIFY thumbsVisibleChanged)

public:

    // Static Constructor
    static MainViewerWindow* getInstance();
    // Release Instance
    void release();

    // Set Thumbs Visible
    void setThumbsVisible(const bool& aVisible);
    // Get Thumbs Visible
    bool getThumbsVisible();

public slots:

    // Launch Browser Window
    void launchBrowserWindow();

    // Launc Preferences
    void launcPreferences();

    // Show Window
    void showWindow();

    // Hide Window
    void hideWindow();

    // Show Status Text
    void showStatusText(const QString& aStatusText, const int& aTimeout = 0);

signals:

    // Thumbnails Visible Changed Signal
    void thumbsVisibleChanged(const bool& aThumbsVisible);

protected:

    // Constructor
    explicit MainViewerWindow(QWidget* aParent = NULL);

    // Destructor
    virtual ~MainViewerWindow();

protected:
    friend class MainBrowserWindow;

    // Get QML Context
    QQmlContext* getQMLContext();

private slots:

    // Current File Changed Slot
    void currentFileChanged(const QString& aFilePath);

    // On Action About Triggered
    void on_actionAbout_triggered();
    // On Action Preferences Triggered
    void on_actionPreferences_triggered();
    // On Action Rename Triggered
    void on_actionRename_triggered();
    // On Action Delete Triggered
    void on_actionDelete_triggered();
    // On Action Rotate Left Triggered
    void on_actionRotate_Left_triggered();
    // On Action Rotate Rigth Triggered
    void on_actionRotate_Right_triggered();
    // On Action Flip Horizontally Triggered
    void on_actionFlip_Horizontally_triggered();
    // On Action Flip Vertically Triggered
    void on_actionFlip_Vertically_triggered();
    // On Action Quit Triggered
    void on_actionQuit_triggered();
    // On Action Slide Show Triggered
    void on_actionSlide_Show_triggered();

private:

    // Restore UI
    void restoreUI();

    // Save Settings
    void saveSettings();

private:

    // UI
    Ui::MainViewerWindow*   ui;

    // Main Browser Window
    MainBrowserWindow*      mainBrowserWindow;

    // Thumbnails Visible
    bool                    thumbsVisible;
};

#endif // MAINVIEWERWINDOW_H
