#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class PreferencesDialog;
}

class MainBrowserWindow;

//==============================================================================
// Preferences Dialog
//==============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit PreferencesDialog(QWidget* aParent = NULL);

    // Launch Dialog
    void launchDialog();

    // Destructor
    virtual ~PreferencesDialog();

private slots:

    // Slide Show Delay Slider Value Changed
    void on_slideShowDelaySlider_valueChanged(int value);

    // Dialog Accepted
    void on_buttonBox_accepted();
    // Button Clicked
    void on_buttonBox_clicked(QAbstractButton* aButton);
    // Dialog Rejected
    void on_buttonBox_rejected();

    void on_registerFileTypesButton_clicked();

private:

    // Load Settings
    void loadSettings();
    // Apply Settings
    void applySettins();
    // Save Settings
    void saveSettings();

private:

    // UI
    Ui::PreferencesDialog*  ui;
    // Main Browser Window
    MainBrowserWindow*      mainBrowserWindow;
};

#endif // PREFERENCESDIALOG_H
