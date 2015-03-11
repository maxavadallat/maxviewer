#include "confirmationdialog.h"
#include "ui_confirmationdialog.h"


//==============================================================================
// Constructor
//==============================================================================
ConfirmationDialog::ConfirmationDialog(QWidget* aParent)
    : QDialog(aParent),
    ui(new Ui::ConfirmationDialog)
{
    ui->setupUi(this);
}

//==============================================================================
// Set Confirmation Text
//==============================================================================
void ConfirmationDialog::setConfirmationText(const QString& aConfirmationText)
{
    // Set Text
    ui->confirmationTextLabel->setText(aConfirmationText);
}

//==============================================================================
// Destructor
//==============================================================================
ConfirmationDialog::~ConfirmationDialog()
{
    delete ui;
}
