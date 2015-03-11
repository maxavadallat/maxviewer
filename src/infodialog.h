#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>

namespace Ui {
class InfoDialog;
}

//==============================================================================
// Info Dialog Class
//==============================================================================
class InfoDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit InfoDialog(QWidget* aParent = NULL);

    // Set Text
    void setText(const QString& aText);

    // Show Dialog With Text
    void showDialogWithText(const QString& aText);

    // Destructor
    virtual ~InfoDialog();

private:

    // UI
    Ui::InfoDialog*     ui;
};

#endif // INFODIALOG_H
