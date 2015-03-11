#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QWidget>

namespace Ui {
class AboutForm;
}

//==============================================================================
// About Form
//==============================================================================
class AboutForm : public QWidget
{
    Q_OBJECT

public:
    // Constructor
    explicit AboutForm(QWidget* aParent = NULL);

    // Destructor
    virtual ~AboutForm();

private slots:
    void on_pushButton_clicked();

private:

    // UI
    Ui::AboutForm*  ui;
};

#endif // ABOUTFORM_H
