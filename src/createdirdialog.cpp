#include "createdirdialog.h"
#include "ui_createdirdialog.h"

//==============================================================================
// Constructor
//==============================================================================
CreateDirDialog::CreateDirDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CreateDirDialog)
{
    // Setup UI
    ui->setupUi(this);
    // Set Focus
    ui->newDirNameEdit->setFocus();
}

//==============================================================================
// Get New Dir Name
//==============================================================================
QString CreateDirDialog::getNewDirName()
{
    return ui->newDirNameEdit->text();
}

//==============================================================================
// Destructor
//==============================================================================
CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}
