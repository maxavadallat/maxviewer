#include "renameseriesdialog.h"
#include "ui_renameseriesdialog.h"

//==============================================================================
// Constructor
//==============================================================================
RenameSeriesDialog::RenameSeriesDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::RenameSeriesDialog)
{
    // Setup UI
    ui->setupUi(this);
}

//==============================================================================
// Get Start Index
//==============================================================================
int RenameSeriesDialog::getStartIndex()
{
    return ui->startIndexSpinBox->value();
}

//==============================================================================
// Get Pattern
//==============================================================================
QString RenameSeriesDialog::getPattern()
{
    return ui->patternEditor->text();
}

//==============================================================================
// Exec
//==============================================================================
int RenameSeriesDialog::exec()
{
    // Select All
    ui->patternEditor->selectAll();

    // Set Focus
    ui->patternEditor->setFocus();

    return QDialog::exec();
}

//==============================================================================
// Destructor
//==============================================================================
RenameSeriesDialog::~RenameSeriesDialog()
{
    delete ui;
}
