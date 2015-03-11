#include "aboutform.h"
#include "constants.h"

#include "ui_aboutform.h"

//==============================================================================
// Constructor
//==============================================================================
AboutForm::AboutForm(QWidget* aParent)
    : QWidget(aParent)
    , ui(new Ui::AboutForm)
{
    ui->setupUi(this);
}

//==============================================================================
// Close Button Clicked
//==============================================================================
void AboutForm::on_pushButton_clicked()
{
    // Close
    close();
}

//==============================================================================
// Destructor
//==============================================================================
AboutForm::~AboutForm()
{
    delete ui;
}

