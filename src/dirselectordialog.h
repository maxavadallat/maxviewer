#ifndef DIRSELECTORDIALOG_H
#define DIRSELECTORDIALOG_H

#include <QDialog>

namespace Ui {
class DirSelectorDialog;
}

//==============================================================================
// Dir Selector Dialog
//==============================================================================
class DirSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit DirSelectorDialog(QWidget* aParent = NULL);

    // Get Selected Dir
    QString getSelectedDir();

    // Destructor
    virtual ~DirSelectorDialog();

private slots:

    // Add Dir Button Clicked
    void on_addDirButton_clicked();
    // Edit Dir Button Clicked
    void on_editDirButton_clicked();
    // Delete Dir Button Clicked
    void on_deleteButton_clicked();
    // Dialog Accepted
    void on_DirSelectorDialog_accepted();
    // Dialog Rejected
    void on_DirSelectorDialog_rejected();

private:

    // Load Dirs
    void loadDirs();
    // Save Dirs
    void saveDirs();

    // Find Index
    int findIndex(const QString& aDirPath);

private:
    // UI
    Ui::DirSelectorDialog*  ui;
    // Selected Dir
    QString                 selectedDir;
};

#endif // DIRSELECTORDIALOG_H
