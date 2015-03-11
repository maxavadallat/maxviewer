#include <QFile>
#include <QDebug>

#include "comparedialog.h"
#include "ui_comparedialog.h"

//==============================================================================
// Constructor
//==============================================================================
CompareDialog::CompareDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::CompareDialog)
{
    ui->setupUi(this);

    // Connect Signals
    connect(ui->leftImageFrame, SIGNAL(imageUpdated()), this, SLOT(leftImageChanged()));
    connect(ui->rightImageFrame, SIGNAL(imageUpdated()), this, SLOT(rightImageChanged()));

    // ...
}

//==============================================================================
// Configure Buttons
//==============================================================================
void CompareDialog::configureButtons(QDialogButtonBox::StandardButtons aButtons)
{
    //qDebug() << "CompareDialog::configureButtons";

    // Set Standard Buttons
    ui->buttonBox->setStandardButtons(aButtons);

}

//==============================================================================
// Set Left Image
//==============================================================================
void CompareDialog::setLeftImage(const QString& aImageFile)
{
    // Set Left Image
    ui->leftImageFrame->loadImage(aImageFile);
}

//==============================================================================
// Set Right Image
//==============================================================================
void CompareDialog::setRightImage(const QString& aImageFile)
{
    // Set Right Image
    ui->rightImageFrame->loadImage(aImageFile);
}

//==============================================================================
// Set Current Dir
//==============================================================================
void CompareDialog::setCurrentDir(const QString& aCurrentDir)
{
    // Set Left & Right Frame Current Dir
    ui->leftImageFrame->setCurrentDir(aCurrentDir);
    ui->rightImageFrame->setCurrentDir(aCurrentDir);
}

//==============================================================================
// Left Image Changed Slot
//==============================================================================
void CompareDialog::leftImageChanged()
{
    //qDebug() << "CompareDialog::leftImageChanged";

    // Hide Instructions Label
    ui->leftInstructionsLabel->setVisible(false);

    // Load Image To Center Frame
    ui->centerFrame->setLeftImage(ui->leftImageFrame->getFileName());
}

//==============================================================================
// Right Image Changed Slot
//==============================================================================
void CompareDialog::rightImageChanged()
{
    //qDebug() << "CompareDialog::leftImageChanged";

    // Hide Instructions Label
    ui->rightInstructionsLabel->setVisible(false);

    // Load Image To Center Frame
    ui->centerFrame->setRightImage(ui->rightImageFrame->getFileName());
}

//==============================================================================
// Destructor
//==============================================================================
CompareDialog::~CompareDialog()
{
    delete ui;
}
