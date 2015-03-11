#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

namespace Ui {
class CompareDialog;
}

class ImageCompareFrame;

//==============================================================================
// Compare Dialog Class
//==============================================================================
class CompareDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor
    explicit CompareDialog(QWidget* aParent = NULL);

    // Configure Buttons
    void configureButtons(QDialogButtonBox::StandardButtons aButtons);

    // Set Left Image
    void setLeftImage(const QString& aImageFile);

    // Set Right Image
    void setRightImage(const QString& aImageFile);

    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);

    // Destructor
    virtual ~CompareDialog();

protected slots:

    // Left Image Changed Slot
    void leftImageChanged();

    // Right Image Changed Slot
    void rightImageChanged();

private:

    // UI
    Ui::CompareDialog*      ui;
};

#endif // COMPAREDIALOG_H
