#ifndef RENAMESERIESDIALOG_H
#define RENAMESERIESDIALOG_H

#include <QDialog>

namespace Ui {
class RenameSeriesDialog;
}

//==============================================================================
// Rename Series Dialog
//==============================================================================
class RenameSeriesDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit RenameSeriesDialog(QWidget* aParent = NULL);

    // Get Start Index
    int getStartIndex();

    // Get Pattern
    QString getPattern();

    // Exec
    virtual int exec();

    // Destructor
    virtual ~RenameSeriesDialog();

private:

    // UI
    Ui::RenameSeriesDialog*     ui;
};

#endif // RENAMESERIESDIALOG_H
