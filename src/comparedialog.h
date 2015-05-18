#ifndef COMPAREDIALOG_H
#define COMPAREDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include <QPushButton>

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

    // Add Button
    void addButton(const QString& aButton);

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

    // Button Box Button Clicked
    void buttonBoxButtonClicked(QAbstractButton* aButton);

private:
    friend class MainBrowserWindow;

    // UI
    Ui::CompareDialog*      ui;

    // Custom Action Index
    int                     actionIndex;
};

#endif // COMPAREDIALOG_H
