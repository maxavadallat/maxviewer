#include "infodialog.h"
#include "ui_infodialog.h"

//==============================================================================
// Constructor
//==============================================================================
InfoDialog::InfoDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::InfoDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Text
//==============================================================================
void InfoDialog::setText(const QString& aText)
{
    // Set Info Text Label Text
    ui->infoTextLabel->setText(aText);
}

//==============================================================================
// Show Dialog With Text
//==============================================================================
void InfoDialog::showDialogWithText(const QString& aText)
{
    // Set Text
    setText(aText);

    // Exec
    exec();
}

//==============================================================================
// Destructor
//==============================================================================
InfoDialog::~InfoDialog()
{
    delete ui;
}
