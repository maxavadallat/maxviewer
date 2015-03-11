
#include <QQmlContext>
#include <QDebug>

#include "mainbrowserwindow.h"
#include "duplicatelistmodel.h"
#include "constants.h"

#include "duplicatesearchresultdialog.h"
#include "ui_duplicatesearchresultdialog.h"


//==============================================================================
// Constructor
//==============================================================================
DuplicateSearchResultDialog::DuplicateSearchResultDialog(QWidget* aParent)
    : QDialog(aParent)
    , ui(new Ui::DuplicateSearchResultDialog)
    , duplicateListModel(NULL)
{
    // Setup UI
    ui->setupUi(this);

    // ...
}

//==============================================================================
// Show Dialog
//==============================================================================
int DuplicateSearchResultDialog::showDialog(DuplicateListModel* aModel)
{
    // Set Model
    duplicateListModel = aModel;

    // Check Duplicate Model
    if (duplicateListModel) {
        // Clear Button Box
        ui->buttonBox->clear();
        // Add Only Close Button
        ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);

        // Check Row Count
        if (duplicateListModel->rowCount() > 0) {
            // Get QML Context
            QQmlContext* ctxt = ui->duplicateListWidget->rootContext();
            // Set Context Property
            ctxt->setContextProperty(DUPLICATE_LIST_MODEL, aModel);
            // Set QML Source
            ui->duplicateListWidget->setSource(QUrl("qrc:/qml/DuplicateList.qml"));
            // Set No Duplicates Label Visibility
            ui->noDuplicatesLabel->setVisible(false);
            // Add Button
            ui->buttonBox->addButton("Clear Duplicates", QDialogButtonBox::AcceptRole);

        } else {
            // Set Duplicates List Widget Visibility
            ui->duplicateListWidget->setVisible(false);
            // Set No Duplicates Label Visibility
            ui->noDuplicatesLabel->setVisible(true);
        }

        // Exec
        return exec();
    }

    return QDialog::Rejected;
}

//==============================================================================
// Destructor
//==============================================================================
DuplicateSearchResultDialog::~DuplicateSearchResultDialog()
{
    delete ui;
}
