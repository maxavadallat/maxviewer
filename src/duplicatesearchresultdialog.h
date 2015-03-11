#ifndef DUPLICATESEARCHRESULTDIALOG_H
#define DUPLICATESEARCHRESULTDIALOG_H

#include <QDialog>

namespace Ui {
class DuplicateSearchResultDialog;
}

class MainBrowserWindow;
class DuplicateListModel;

//==============================================================================
// Duplicate Search REsult Dialog
//==============================================================================
class DuplicateSearchResultDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit DuplicateSearchResultDialog(QWidget* aParent = NULL);

    // Show Dialog
    int showDialog(DuplicateListModel* aModel);

    // Destructor
    virtual ~DuplicateSearchResultDialog();

private:

    // UI
    Ui::DuplicateSearchResultDialog*    ui;

    // Duplicate List Model
    DuplicateListModel*                 duplicateListModel;

};

#endif // DUPLICATESEARCHRESULTDIALOG_H
