#include "renamefiledialog.h"
#include "ui_renamefiledialog.h"

//==============================================================================
// Constructor
//==============================================================================
RenameFileDialog::RenameFileDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::RenameFileDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set File Name
//==============================================================================
void RenameFileDialog::setFileName(const QString& aFileName)
{
    // Set Text
    ui->fileNameEditor->setText(aFileName);
    // Select All Text
    ui->fileNameEditor->selectAll();
}

//==============================================================================
// Get File Name
//==============================================================================
QString RenameFileDialog::getFileName()
{
    return ui->fileNameEditor->text();
}

//==============================================================================
// Exec
//==============================================================================
int RenameFileDialog::exec()
{
    // Set Focus
    ui->fileNameEditor->setFocus();

    return QDialog::exec();
}

//==============================================================================
// Destructor
//==============================================================================
RenameFileDialog::~RenameFileDialog()
{
    delete ui;
}
