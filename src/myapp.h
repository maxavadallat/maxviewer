#ifndef MYAPP
#define MYAPP

#include <QApplication>
#include <QEvent>
#include <QFileOpenEvent>
#include <QString>


//==============================================================================
// My Application Class
//==============================================================================
class MyApplication : public QApplication
{
    Q_OBJECT

public:
    // Constructor
    MyApplication(int& argc, char**argv);

    // Get Open File Argument
    QString getOpenFileArgument();

signals:

    // File Open Event Occured Signal
    void fileOpenEventOccured(const QString& aArgument);

protected:
    // Event Handling
    bool event(QEvent* event);

private:

    // Open File Argument
    QString     openFileArgument;
};

#endif // MYAPP

