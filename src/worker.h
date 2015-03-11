#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class MainBrowserWindow;

//==============================================================================
// Worker Class Operation Types
//==============================================================================
enum OperationType
{
    OTRotateFilesLeft   = 0,
    OTRotateFilesRight,
    OTFlipFilesHorizontally,
    OTFlipFilesVertically,
    OTDeleteFiles,
    OTRenameFiles,
    OTCopyToFiles,
    OTMoveToFiles,
    OTFindDuplicates
};


//==============================================================================
// Worker Class For Thread Operations
//==============================================================================
class Worker : public QObject
{
    Q_OBJECT

public:

    // Constructor
    explicit Worker(QObject* aParent = NULL);

    // Destructor
    virtual ~Worker();

signals:

    // Populate Browser Model
    void populateBrowserModel(const int& newIndex);
    // Result Ready Signal
    void resultReady(const int& aOperation, const int& aResult);
    // File Renames Signal
    void fileRenamed(const QString& aFileName);
    // Refresh View
    void refreshView();

public slots:

    // Do Work
    void doWork(const int& aOperation);

    // Stop
    void stop();

private:
    // Main Browser Window
    MainBrowserWindow*      mainBrowserWindow;
};

#endif // WORKER_H
