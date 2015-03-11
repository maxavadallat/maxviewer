#ifndef CONFIRMATIONDIALOG_H
#define CONFIRMATIONDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmationDialog;
}

//==============================================================================
// Confirmation Dialog
//==============================================================================
class ConfirmationDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit ConfirmationDialog(QWidget* aParent = NULL);

    // Set Confirmation Text
    void setConfirmationText(const QString& aConfirmationText);

    // Destructor
    virtual ~ConfirmationDialog();

private:

    // UI
    Ui::ConfirmationDialog*     ui;
};

#endif // CONFIRMATIONDIALOG_H
