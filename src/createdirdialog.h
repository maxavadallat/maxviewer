#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirDialog;
}

//==============================================================================
// Create Dir Dialog
//==============================================================================
class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit CreateDirDialog(QWidget* aParent = NULL);

    // Get New Dir Name
    QString getNewDirName();

    // Destructor
    virtual ~CreateDirDialog();

private:
    // UI
    Ui::CreateDirDialog* ui;
};

#endif // CREATEDIRDIALOG_H
