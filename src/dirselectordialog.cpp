#include <QDir>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

#include <QDebug>

#include "dirselectordialog.h"
#include "constants.h"
#include "ui_dirselectordialog.h"


//==============================================================================
// Constructor
//==============================================================================
DirSelectorDialog::DirSelectorDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DirSelectorDialog)
    , selectedDir(QString(""))
{
    //qDebug() << "DirSelectorDialog::DirSelectorDialog";

    // Setup UI
    ui->setupUi(this);
    // Check Count
    if (ui->dirListWidget->count() > 0) {
        // Set Current Row
        ui->dirListWidget->setCurrentRow(0);
    }
    // Set Focus
    ui->dirListWidget->setFocus();

    // Load Dirs
    loadDirs();
}

//==============================================================================
// Get Selected Dir
//==============================================================================
QString DirSelectorDialog::getSelectedDir()
{
    return selectedDir;
}

//==============================================================================
// Load Dirs
//==============================================================================
void DirSelectorDialog::loadDirs()
{
    // Init Dir List File
    QFile dirListFile(QDir::homePath() + QString("/") + DEFAULT_TRANSFER_LIST_FILE_NAME);

    // Open File
    if (dirListFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "DirSelectorDialog::loadDirs";

        // Init Text Stream
        QTextStream textStream(&dirListFile);

        // Go Thru Dir List File
        while (!textStream.atEnd()) {
            // Read Line
            QString line = textStream.readLine();
            // Append To List
            ui->dirListWidget->addItem(line);
        }

        // Close File
        dirListFile.close();
    } else {
        qDebug() << "DirSelectorDialog::loadDirs - ERROR OPENING FILE FOR SAVE!";
    }

    // ...
}

//==============================================================================
// Save Dirs
//==============================================================================
void DirSelectorDialog::saveDirs()
{
    // Get Dir List Count
    int dlCount = ui->dirListWidget->count();

    // Init Dir List File
    QFile dirListFile(QDir::homePath() + QString("/") + DEFAULT_TRANSFER_LIST_FILE_NAME);

    // Delete Dir List File
    if (dirListFile.exists() && !dirListFile.remove()) {
        qDebug() << "DirSelectorDialog::saveDirs - ERROR REMOVING DIR LIST FILE!";

        return;
    }

    // Check Count
    if (dlCount > 0) {
        // Open Dir List File
        if (dirListFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "DirSelectorDialog::saveDirs";

            // Init Text Stream
            QTextStream textStream(&dirListFile);

            // Go Thru Dir List Widget Items
            for (int i=0; i<dlCount; i++) {
                // Write Line
                textStream << ui->dirListWidget->item(i)->text();
                // Write Line End
                textStream << "\n";
            }

            // Close File
            dirListFile.close();
        } else {
            qDebug() << "DirSelectorDialog::saveDirs - ERROR OPENING FILE FOR SAVE!";
        }
    }
 }

//==============================================================================
// Find Index
//==============================================================================
int DirSelectorDialog::findIndex(const QString& aDirPath)
{
    // Get Items Count
    int dlCount = ui->dirListWidget->count();

    // Go Thru Items
    for (int i=0; i<dlCount; i++) {
        // Get Item
        QListWidgetItem* item = ui->dirListWidget->item(i);

        // Check Dir
        if (item->text() == aDirPath) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Add Dir Button Clicked
//==============================================================================
void DirSelectorDialog::on_addDirButton_clicked()
{
    // Init File Dialog
    QFileDialog fileDialog;
    // Set Dir
    fileDialog.setDirectory(QDir::homePath());
    // Set Filter
    fileDialog.setFilter(QDir::AllDirs | QDir::NoDot);

    // Exec Dialog
    if (fileDialog.exec()) {

        qDebug() << "DirSelectorDialog::on_addDirButton_clicked - path: " << fileDialog.directory().path();

        // Check If Dir Already Added
        int foundIndex = findIndex(fileDialog.directory().path());

        // Check Found Index
        if (foundIndex < 0) {
            // Add Dir
            ui->dirListWidget->insertItem(0, fileDialog.directory().path());
            // Set Current Row
            ui->dirListWidget->setCurrentRow(0);

            // ...
        }
    }
}

//==============================================================================
// Edit Dir Button Clicked
//==============================================================================
void DirSelectorDialog::on_editDirButton_clicked()
{
    // Get Current Item
    QListWidgetItem* currItem = ui->dirListWidget->currentItem();

    // Check Current Item
    if (currItem) {
        // Init File Dialog
        QFileDialog fileDialog;

        // Set File Dialog Dir
        fileDialog.setDirectory(currItem->text());

        // Set Filter
        fileDialog.setFilter(QDir::AllDirs | QDir::NoDot);

        // Exec Dialog
        if (fileDialog.exec()) {
            // Get Index
            int foundIndex = findIndex(fileDialog.directory().path());
            // Check Index
            if (foundIndex < 0) {
                // Update Dir
                currItem->setText(fileDialog.directory().path());
            }

            // ...
        }
    }
}

//==============================================================================
// Delete Dir Button Clicked
//==============================================================================
void DirSelectorDialog::on_deleteButton_clicked()
{
    // Get Current Item
    QListWidgetItem* currItem = ui->dirListWidget->currentItem();

    // Check Current Item
    if (currItem) {
        // Get Current Row
        int currRow = ui->dirListWidget->currentRow();

        // Delete Item
        ui->dirListWidget->model()->removeRow(currRow);

        // Set New Current Row

        // ...
    }
}

//==============================================================================
// Dialog Accepted
//==============================================================================
void DirSelectorDialog::on_DirSelectorDialog_accepted()
{
    // Save Dir List
    saveDirs();

    // Set Selected Dir
    selectedDir = ui->dirListWidget->count() > 0 ? ui->dirListWidget->currentItem()->text() : QString("");
}

//==============================================================================
// Dialog Rejected
//==============================================================================
void DirSelectorDialog::on_DirSelectorDialog_rejected()
{
    // Clear Selected Dir
    selectedDir = QString("");
}

//==============================================================================
// Destructor
//==============================================================================
DirSelectorDialog::~DirSelectorDialog()
{
    delete ui;

    //qDebug() << "DirSelectorDialog::~DirSelectorDialog";
}

