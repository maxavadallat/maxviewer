#ifndef RENAMEFILEDIALOG_H
#define RENAMEFILEDIALOG_H

#include <QDialog>

namespace Ui {
class RenameFileDialog;
}

//==============================================================================
// REname File Dialog Class
//==============================================================================
class RenameFileDialog : public QDialog
{
    Q_OBJECT

public:

    // Constructor
    explicit RenameFileDialog(QWidget* aParent = NULL);

    // Set File Name
    void setFileName(const QString& aFileName);

    // Get File Name
    QString getFileName();

    // Exec
    virtual int exec();

    // Destructor
    virtual ~RenameFileDialog();

private:

    // UI
    Ui::RenameFileDialog*   ui;
};

#endif // RENAMEFILEDIALOG_H
