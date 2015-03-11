
#include <QPushButton>
#include <QSettings>
#include <QDebug>

#include "mainbrowserwindow.h"
#include "preferencesdialog.h"
#include "constants.h"
#include "ui_preferencesdialog.h"

//==============================================================================
// Constructor
//==============================================================================
PreferencesDialog::PreferencesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PreferencesDialog)
    , mainBrowserWindow(MainBrowserWindow::getInstance())
{
    ui->setupUi(this);
}

//==============================================================================
// Launch Dialog
//==============================================================================
void PreferencesDialog::launchDialog()
{
    //qDebug() << "PreferencesDialog::launchDialog";

    // Load Settings
    loadSettings();

    // Set Modal
    setModal(true);

    // Exec
    exec();
}

//==============================================================================
// Load Settings
//==============================================================================
void PreferencesDialog::loadSettings()
{
    qDebug() << "PreferencesDialog::loadSettings";

    // Init Settings
    QSettings settings;

    // Get Slide Show Delay
    int ssDelay = settings.allKeys().contains(SETTINGS_KEY_SLIDE_SHOW_DELAY) ? settings.value(SETTINGS_KEY_SLIDE_SHOW_DELAY).toInt() : DEFAULT_SLIDE_SHOW_DELAY;

    // Set Slide Show Delay Value Label
    ui->slideShowDelayValueLabel->setText(QString("%1 secs").arg(ssDelay / 1000));
    // Set Slide Show Dealy Slider
    ui->slideShowDelaySlider->setValue(ssDelay);
    // Set Slide Show Wrap Around Check Box
    ui->slideShowWrapAroundCheckBox->setChecked(settings.value(SETTINGS_KEY_SLIDE_SHOW_WRAPAROUND).toBool());

    // Get Slide Show Direction
    int ssDirection = settings.allKeys().contains(SETTINGS_KEY_SLIDE_SHOW_DIRECTION) ?  settings.value(SETTINGS_KEY_SLIDE_SHOW_DIRECTION).toInt() : DEFAULT_SLIDE_SHOW_DIRECTION;

    // Switch Slide Sho Direction
    switch (ssDirection) {
        default:
        case SSDForward:
            // Set Checked
            ui->forwardButton->setChecked(true);
        break;

        case SSDReversed:
            // Set Checked
            ui->reversedButton->setChecked(true);
        break;

        case SSDRandom:
            // Set Checked
            ui->randomButton->setChecked(true);
        break;
    }

    // Get Thumbs Width
    int thumbsWidth = settings.allKeys().contains(SETTINGS_KEY_THUMBS_WIDTH) ? settings.value(SETTINGS_KEY_THUMBS_WIDTH).toInt() : DEFAULT_THUMBS_WIDTH;

    // Set Thumbs Width Spinner
    ui->widthSpinBox->setValue(thumbsWidth);

    // Get Thumbs Height
    int thumbsHeight = settings.allKeys().contains(SETTINGS_KEY_THUMBS_HEIGHT) ? settings.value(SETTINGS_KEY_THUMBS_HEIGHT).toInt() : DEFAULT_THUMBS_HEIGHT;

    // Set Thumbs Height Spinner
    ui->heightSpinBox->setValue(thumbsHeight);


    // ...
}

//==============================================================================
// Apply Settings
//==============================================================================
void PreferencesDialog::applySettins()
{
    qDebug() << "PreferencesDialog::applySettins";

    // Set Thumbs Width
    mainBrowserWindow->setThumbsWidth(ui->widthSpinBox->value());

    // Set Thumbs Height
    mainBrowserWindow->setThumbsHeight(ui->heightSpinBox->value());

    // Init Slide Show Direction
    int ssDirection = SSDForward;
    // Check Forward Button
    if (ui->forwardButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDForward;
    }
    // Check Reversed Button
    if (ui->reversedButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDReversed;
    }
    // Check Random Button
    if (ui->randomButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDRandom;
    }

    // Set Slide Show Direction
    mainBrowserWindow->setSlideShowDirection(ssDirection);

    // Set Slide Show Delay
    mainBrowserWindow->setSlideShowDelay(ui->slideShowDelaySlider->value());

    // Set Slide Show Wrap Around
    mainBrowserWindow->setSlideShowWrap(ui->slideShowWrapAroundCheckBox->isChecked());

    // ...

}

//==============================================================================
// Save Settings
//==============================================================================
void PreferencesDialog::saveSettings()
{
    qDebug() << "PreferencesDialog::saveSettings";

    // Init Settings
    QSettings settings;

    // Save Slide Show Delay Value
    settings.setValue(SETTINGS_KEY_SLIDE_SHOW_DELAY, ui->slideShowDelaySlider->value());

    // Init Slide Show Direction
    int ssDirection = SSDForward;
    // Check Forward Button
    if (ui->forwardButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDForward;
    }
    // Check Reversed Button
    if (ui->reversedButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDReversed;
    }
    // Check Random Button
    if (ui->randomButton->isChecked()) {
        // Set Slide Show Direction
        ssDirection = SSDRandom;
    }

    // Save Slide Show Direction Value
    settings.setValue(SETTINGS_KEY_SLIDE_SHOW_DIRECTION, ssDirection);

    // Save Slide Show Wrap Around
    settings.setValue(SETTINGS_KEY_SLIDE_SHOW_WRAPAROUND, ui->slideShowWrapAroundCheckBox->isChecked());

    // Save Thumbs Width
    settings.setValue(SETTINGS_KEY_THUMBS_WIDTH, ui->widthSpinBox->value());
    // Save Thumbs Height
    settings.setValue(SETTINGS_KEY_THUMBS_HEIGHT, ui->heightSpinBox->value());

    // ...
}

//==============================================================================
// Slide Show Delay Slider Value Changed
//==============================================================================
void PreferencesDialog::on_slideShowDelaySlider_valueChanged(int aValue)
{
    // Set Slide Show Delay Value Label
    ui->slideShowDelayValueLabel->setText(QString("%1 secs").arg(aValue / 1000));
}

//==============================================================================
// Dialog Accepted
//==============================================================================
void PreferencesDialog::on_buttonBox_accepted()
{
    //qDebug() << "PreferencesDialog::on_buttonBox_accepted";

    // Apply Settings
    applySettins();

    // Save Settings
    saveSettings();
}

//==============================================================================
// Button Clicked
//==============================================================================
void PreferencesDialog::on_buttonBox_clicked(QAbstractButton* aButton)
{
    // Check Button
    if (aButton == ui->buttonBox->button(QDialogButtonBox::Apply)) {

        //qDebug() << "PreferencesDialog::on_buttonBox_clicked - Apply";

        // Apply Settings
        applySettins();

        // ...

    }
}

//==============================================================================
// Register App For Supported Images
//==============================================================================
void PreferencesDialog::on_registerFileTypesButton_clicked()
{
    //qDebug() << "PreferencesDialog::on_registerFileTypesButton_clicked";

    // Register App for Supported Images
    mainBrowserWindow->registerAppForImages();
}

//==============================================================================
// Dialog Rejected
//==============================================================================
void PreferencesDialog::on_buttonBox_rejected()
{
    //qDebug() << "PreferencesDialog::on_buttonBox_accepted";

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

