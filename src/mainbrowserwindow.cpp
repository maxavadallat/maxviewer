#include <QDir>
#include <QDebug>
#include <QVariant>
#include <QFileInfo>
#include <QFileInfoList>
#include <QQmlContext>
#include <QMutexLocker>
#include <QSettings>

#include "mainbrowserwindow.h"
#include "mainviewerwindow.h"
#include "browserdataobject.h"
#include "preferencesdialog.h"
#include "aboutform.h"
#include "dirselectordialog.h"
#include "renameseriesdialog.h"
#include "renamefiledialog.h"
#include "infodialog.h"
#include "comparedialog.h"
#include "imagecompareframe.h"
#include "createdirdialog.h"
#include "confirmationdialog.h"
#include "duplicatesearchresultdialog.h"
#include "worker.h"
#include "duplicatelistmodel.h"
#include "utility.h"
#include "constants.h"

#include "ui_mainbrowserwindow.h"

// Main Browser Window Singleton
static MainBrowserWindow* mainBrowserWindow = NULL;

//==============================================================================
// Static Constructor
//==============================================================================
MainBrowserWindow* MainBrowserWindow::getInstance()
{
    // Check Singleton
    if (!mainBrowserWindow) {
        // Create Main Browser Window
        mainBrowserWindow = new MainBrowserWindow();
    }

    return mainBrowserWindow;
}

//==============================================================================
// Release Instance
//==============================================================================
void MainBrowserWindow::release()
{
    // Delete Main Browser Window
    delete mainBrowserWindow;
    // Reset Singleton
    mainBrowserWindow = NULL;
}

//==============================================================================
// Constructor
//==============================================================================
MainBrowserWindow::MainBrowserWindow(QWidget* aParent)
    : QMainWindow(aParent)
    , ui(new Ui::MainBrowserWindow)
    , sortingGroup(NULL)
    , fsModel(NULL)
    , currentIndex(-1)
    , popupIndex(-1)
    , viewerWindow(NULL)
    , aboutDialog(NULL)
    , dirSelector(NULL)
    , renameSeriesDialog(NULL)
    , renameFileDialog(NULL)
    , infoDialog(NULL)
    , compareDialog(NULL)
    , dsrDialog(NULL)
    , thumbsWidth(DEFAULT_THUMBS_WIDTH)
    , thumbsHeight(DEFAULT_THUMBS_HEIGHT)
    , slideShowDirection(0)
    , slideShowDelay(DEFAULT_SLIDE_SHOW_DELAY)
    , slideShowWrap(true)
    , sortType(DEFAULT_SORT_TYPE_NAME)
    , reverseOrder(false)
    , worker(NULL)
    , slideShowTimerID(-1)
    , duplicateListModel(NULL)
    , deleteSelectedOnly(false)
{
    // Setup UI
    ui->setupUi(this);

    // Create Action Group
    sortingGroup = new QActionGroup(this);

    // Add Sorting Actions
    sortingGroup->addAction(ui->actionSort_by_Name);
    sortingGroup->addAction(ui->actionSort_by_Type);
    sortingGroup->addAction(ui->actionSort_by_Size);
    sortingGroup->addAction(ui->actionSort_by_Date);
    // Set Exclusive
    sortingGroup->setExclusive(true);

    // Setup Tree View
    setupTreeView();

    // Setup Browser Grid
    setupBrowserGrid();

    // Setup Preview
    setupPreview();

    // Restore UI
    restoreUI();

    // ...
}

//==============================================================================
// Setup Tree View
//==============================================================================
void MainBrowserWindow::setupTreeView()
{
    qDebug() << "MainBrowserWindow::setupTreeView";

    // Create File System Model
    fsModel = new QFileSystemModel;

    // Connect Signal
    connect(fsModel, SIGNAL(directoryLoaded(QString)), this, SLOT(fsModelDirectoryLoaded(QString)));

    // Set File System Model Root Path
    fsModel->setRootPath(QDir::rootPath());
    // Set Filter to Show Only Dirs
    fsModel->setFilter(/*QDir::System |*/ /*QDir::Hidden |*/ QDir::Drives | QDir::AllDirs | QDir::NoDotAndDotDot);

    // Set TreeView Model
    ui->fileSystemTreeView->setModel(fsModel);

    // Get Column Count
    int cCount = fsModel->columnCount();

    // Set All Other Columns Hidden
    for (int i=1; i<cCount; ++i) {
        // Set Columns Hidden
        ui->fileSystemTreeView->setColumnHidden(i, true);
    }

    // Set Context Menu Policy
    ui->fileSystemTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Connect Tree View Signals
    connect(ui->fileSystemTreeView, SIGNAL(currentIndexChanged(QModelIndex)), this, SLOT(treeItemClicked(QModelIndex)));
    connect(ui->fileSystemTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(fileSystemTreeViewCustomContextMenuRequested(QPoint)));

    // ...
}

//==============================================================================
// Setup Browser Grid
//==============================================================================
void MainBrowserWindow::setupBrowserGrid()
{
    qDebug() << "MainBrowserWindow::setupBrowserGrid";

    // Get QML Context
    QQmlContext* qmlContext = ui->browserWidget->rootContext();

    // Set Context Property - Main View Controller
    qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);

    // Set Context Property - Browser Data Model
    qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(browserDataModel));

    // Set Source
    ui->browserWidget->setSource(QUrl("qrc:/qml/browsergrid.qml"));

    // Connect Signals
    connect(ui->browserWidget, SIGNAL(focuseChanged(bool)), this, SIGNAL(browserFocusChanged(bool)));

    connect(ui->browserWidget, SIGNAL(mousePressed(QPoint,Qt::MouseButton)), this, SLOT(browserWidgetMousePressed(QPoint,Qt::MouseButton)));
    connect(ui->browserWidget, SIGNAL(mouseReleased(QPoint,Qt::MouseButton)), this, SLOT(browserWidgetMouseReleased(QPoint,Qt::MouseButton)));

    // ...
}

//==============================================================================
// Setup Preview
//==============================================================================
void MainBrowserWindow::setupPreview()
{
    qDebug() << "MainBrowserWindow::setupPreview";

    // Get QML Context
    QQmlContext* qmlContext = ui->previewWidget->rootContext();

    // Set Context Property - Main View Controller
    qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);

    // Set Source
    ui->previewWidget->setSource(QUrl("qrc:/qml/preview.qml"));

    // ...
}

//==============================================================================
// Restore UI
//==============================================================================
void MainBrowserWindow::restoreUI()
{
    qDebug() << "MainBrowserWindow::restoreUI";

    // Init Settings
    QSettings settings;

    // Get Splitter Value
    QByteArray splitterValue = settings.value(SETTINGS_KEY_MAIN_SPLITTER).toByteArray();

    // Check Splitter Value
    if (!splitterValue.isNull()) {
        // Restore Main Splitter
        ui->mainSplitter->restoreState(splitterValue);
    }

    // Get Splitter Value
    splitterValue = settings.value(SETTINGS_KEY_LEFT_SPLITTER).toByteArray();

    // Check Splitter Value
    if (!splitterValue.isNull()) {
        // Restore Left Pane Splitter
        ui->leftPaneSplitter->restoreState(splitterValue);
    }

    // Get Last Dir From Settings
    QString lastDir = settings.value(SETTINGS_KEY_LAST_DIR).toString();
    // Check Last Dir
    if (!QFile::exists(lastDir)) {
        // Reset Last Dir
        lastDir = QDir::homePath();
    }

    // Set Current Dir
    setCurrentDir(lastDir);

    // Get Last Index
    int lastIndex = settings.value(SETTINGS_KEY_LAST_INDEX).toInt();
    // Set Current Index
    setCurrentIndex(lastIndex);

    // ...

    // Set Auto Scroll
    ui->fileSystemTreeView->setAutoScroll(true);

    // Get Model Index
    QModelIndex modelIndex = fsModel->index(lastDir);

    // Expand
    ui->fileSystemTreeView->expand(modelIndex);
    // Set Current Index
    ui->fileSystemTreeView->setCurrentIndex(modelIndex);

    // Set Focus
    ui->browserWidget->setFocus();

    // Set Thumbs Width
    setThumbsWidth(settings.value(SETTINGS_KEY_THUMBS_WIDTH).toInt());

    // Set Thumbs Height
    setThumbsHeight(settings.value(SETTINGS_KEY_THUMBS_HEIGHT).toInt());

    // Set Slide Show Direction
    setSlideShowDirection(settings.value(SETTINGS_KEY_SLIDE_SHOW_DIRECTION).toInt());

    // Set Slide Show Delay
    setSlideShowDelay(settings.value(SETTINGS_KEY_SLIDE_SHOW_DELAY).toInt());

    // Set Slide Show Wrap Around
    setSlideShowWrap(settings.value(SETTINGS_KEY_SLIDE_SHOW_WRAPAROUND).toBool());

    // ...

    // Get Sort Type
    sortType = settings.value(SETTINGS_KEY_SLIDE_SORT_TYPE, DEFAULT_SORT_TYPE_NAME).toInt();

    // Update Menu
    updateMenu();

    // ...
}

//==============================================================================
// Update Menu
//==============================================================================
void MainBrowserWindow::updateMenu()
{

    // Switch Sorting Type
    switch (sortType) {
        default:
        case DEFAULT_SORT_TYPE_NAME:
            // Update Checked States
            ui->actionSort_by_Name->setChecked(true);
        break;

        case DEFAULT_SORT_TYPE_TYPE:
            // Update Checked States
            ui->actionSort_by_Type->setChecked(true);
        break;

        case DEFAULT_SORT_TYPE_SIZE:
            // Update Checked States
            ui->actionSort_by_Size->setChecked(true);
        break;

        case DEFAULT_SORT_TYPE_DATE:
            // Update Checked States
            ui->actionSort_by_Date->setChecked(true);
        break;
    }
}

//==============================================================================
// Set Current Dir
//==============================================================================
void MainBrowserWindow::setCurrentDir(const QString& aCurrentDir)
{
    // Check Current Dir
    if (currentDir != aCurrentDir) {
        // Stop Worker Thread
        stopWorkerThread();

        // Check If Dir Exists
        if (QFile::exists(aCurrentDir)) {
            //qDebug() << "MainBrowserWindow::setCurrentDir - aCurrentDir: " << aCurrentDir;

            // Set Current Dir
            currentDir = aCurrentDir;
        } else {
            //qDebug() << "MainBrowserWindow::setCurrentDir - homePath: " << QDir::homePath();

            // Set Current Dir
            currentDir = QDir::homePath();
        }

        // Emit Current Dir Changed Signal
        emit currentDirChanged(currentDir);

        // Set Window Title
        setWindowTitle(QString(DEFAULT_APPLICATION_NAME) + QString(" - ") + currentDir);

        // Clear Browser Model
        clearBrowserModel();

        // Update Browser Model
        updateBrowserModel();

        // Show Status Bar Message
        ui->statusBar->showMessage(QString("Current Dir Changed to: %1. %2 Images found.").arg(currentDir).arg(browserDataModel.count()));

        // Get Model Index
        QModelIndex modelIndex = fsModel->index(currentDir);

        // Expand Tree View
        //ui->fileSystemTreeView->expand(modelIndex);

        // Set Current Index
        ui->fileSystemTreeView->setCurrentIndex(modelIndex);
    }
}

//==============================================================================
// Get Current Dir
//==============================================================================
QString MainBrowserWindow::getCurrentDir()
{
    return currentDir;
}

//==============================================================================
// Set Current File
//==============================================================================
void MainBrowserWindow::setCurrentFile(const QString& aCurrentFile)
{
    // Check Current File
    if (currentFile != aCurrentFile) {
        //qDebug() << "MainBrowserWindow::setCurrentFile - aCurrentFile: " << aCurrentFile;
        // Set Current File
        currentFile = aCurrentFile;

        // Emit Current File Changed Signal
        emit currentFileChanged(currentFile);

        // ...
    }
}

//==============================================================================
// Get Current File
//==============================================================================
QString MainBrowserWindow::getCurrentFile()
{
    return currentFile;
}

//==============================================================================
// Set Current Index
//==============================================================================
void MainBrowserWindow::setCurrentIndex(const int& aCurrentIndex)
{
    // Check Current Index
    if (currentIndex != aCurrentIndex || currentIndex >= browserDataModel.count()) {
        //qDebug() << "MainBrowserWindow::setCurrentIndex - aCurrentIndex: " << aCurrentIndex;

        // Set Current Index
        currentIndex = qBound(0, aCurrentIndex, browserDataModel.count() - 1);

        // Emit Current Index Has Been Changed Signal
        emit currentIndexChanged(currentIndex);

        // Set Popup Index
        setPopupIndex(-1);
    }

    // Compose New Current File Name
    QString newCurrentFileName = browserDataModel.count() > 0 ? currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[currentIndex])->fileName : QString("");

    // Set Current File
    setCurrentFile(newCurrentFileName);
}

//==============================================================================
// Get Current Index
//==============================================================================
int MainBrowserWindow::getCurrentIndex()
{
    return currentIndex;
}

//==============================================================================
// Set Popup Index
//==============================================================================
void MainBrowserWindow::setPopupIndex(const int& aPopupIndex)
{
    // Check Popup Index
    if (popupIndex != aPopupIndex) {
        // Set Popup Index
        popupIndex = aPopupIndex;

        // Check Popup Index
        if (popupIndex != -1) {
            //qDebug() << "MainBrowserWindow::setPopupIndex - aPopupIndex: " << aPopupIndex;

            // Emit Popup Index Changed Signal
            emit popupIndexChanged(popupIndex);
        }
    }
}

//==============================================================================
// Get Popup Index
//==============================================================================
int MainBrowserWindow::getPopupIndex()
{
    return popupIndex;
}

//==============================================================================
// Init Worker
//==============================================================================
void MainBrowserWindow::initWorker()
{
    // Check Worker Thread
    if (workerThread.isRunning()) {
        // Terminate Worker Thread
        workerThread.terminate();
    }

    // Check Worker
    if (!worker) {
        qDebug() << "MainBrowserWindow::initWorker";
        // Create Worker
        worker = new Worker();

        // Connect Signals
        connect(worker, SIGNAL(resultReady(int,int)), this, SLOT(workerResultReady(int,int)), Qt::QueuedConnection);
        connect(worker, SIGNAL(populateBrowserModel(int)), this, SLOT(populateBrowserModel(int)));
        connect(this, SIGNAL(operateWorker(int)), worker, SLOT(doWork(int)));
        connect(worker, SIGNAL(fileRenamed(QString)), this, SLOT(fileRenamed(QString)));
        connect(worker, SIGNAL(refreshView()), this, SLOT(refreshView()));

        // ...

        // Move To Thread
        worker->moveToThread(&workerThread);
    }

    // Start Worker Thread
    workerThread.start();
}

//==============================================================================
// Save Settings
//==============================================================================
void MainBrowserWindow::saveSettings()
{
    qDebug() << "MainBrowserWindow::saveSettings";

    // Init Settings
    QSettings settings;

    // Save Last Dir
    settings.setValue(SETTINGS_KEY_LAST_DIR, currentDir);

    // Save Last File
    settings.setValue(SETTINGS_KEY_LAST_FILE, currentFile);

    // Save Last Index
    settings.setValue(SETTINGS_KEY_LAST_INDEX, currentIndex);

    // Save Main Splitter State
    settings.setValue(SETTINGS_KEY_MAIN_SPLITTER, ui->mainSplitter->saveState());

    // Save Left Splitter
    settings.setValue(SETTINGS_KEY_LEFT_SPLITTER, ui->leftPaneSplitter->saveState());

    // Get Maximized State
    bool browserMaximized = windowState() == Qt::WindowMaximized;

    // Save Maximized State
    settings.setValue(SETTINGS_KEY_BROWSER_MAXIMIZED, browserMaximized);

    // Save Sorting Type
    settings.setValue(SETTINGS_KEY_SLIDE_SORT_TYPE, sortType);

    // Save Reverse Order
    settings.setValue(SETTINGS_KEY_SLIDE_SORT_ORDER, reverseOrder);

    // ...
}

//==============================================================================
// Update Browser Model
//==============================================================================
void MainBrowserWindow::updateBrowserModel()
{
    // Show Status Bar Message
    ui->statusBar->showMessage(QString("Updating Browser Data Model..."));

    qDebug() << "MainBrowserWindow::updateBrowserModel - currentDir: " << currentDir;

    // Init Current Dir List
    QDir currentDirEngine(currentDir);

    // Init Filters
    QDir::Filter filters = QDir::Files;

    // Check Settings For All Files

    // ...

    // Init Name Filters
    QStringList nameFilters;

    // Add Filters
    nameFilters << QString(DEFAULT_FILTER_TEMPLATE).arg(DEFAULT_SUPPORTED_FORMAT_BMP);
    nameFilters << QString(DEFAULT_FILTER_TEMPLATE).arg(DEFAULT_SUPPORTED_FORMAT_JPG);
    nameFilters << QString(DEFAULT_FILTER_TEMPLATE).arg(DEFAULT_SUPPORTED_FORMAT_JPEG);
    nameFilters << QString(DEFAULT_FILTER_TEMPLATE).arg(DEFAULT_SUPPORTED_FORMAT_GIF);
    nameFilters << QString(DEFAULT_FILTER_TEMPLATE).arg(DEFAULT_SUPPORTED_FORMAT_PNG);

    // Init Settings
    QSettings settings;

    // Get Sort Type Settings
    sortType = settings.value(SETTINGS_KEY_SLIDE_SORT_TYPE, DEFAULT_SORT_TYPE_NAME).toInt();

    // Init Sort Flags
    QDir::SortFlags sortFlags = (QDir::SortFlag)sortType;

    // Get Reverse Order
    reverseOrder = settings.value(SETTINGS_KEY_SLIDE_SORT_ORDER, false).toBool();

    // Check Sort Type
    if (sortType == DEFAULT_SORT_TYPE_DATE) {
        // Adjust Sort Flags
        sortFlags |= (reverseOrder ? (QDir::SortFlag)0 : QDir::Reversed);
    } else {
        // Adjust Sort Flags
        sortFlags |= (reverseOrder ? QDir::Reversed : (QDir::SortFlag)0);
    }

    // Get File Info List
    QFileInfoList fileInfoList = currentDirEngine.entryInfoList(nameFilters, filters, sortFlags);

    // Get File Info List Count
    int filCount = fileInfoList.count();

    //qDebug() << "MainBrowserWindow::updateBrowserModel - found: " << filCount;

    // Init Update Percent
    int percent = 0;

    // Go Thru Result List
    for (int i=0; i<filCount; ++i) {
        // Get File Name
        QString fileName = fileInfoList[i].fileName();

        //qDebug() << "MainBrowserWindow::updateBrowserModel - fileName: " << fileName;

        // Add Items To Browser Data Model
        browserDataModel << new BrowserDataObject(fileName, FTFile);

        // update Percent
        percent = i * 100 / filCount;

        // Show Status Bar Message
        //ui->statusBar->showMessage(QString("Updating Browser Data Model...%1%").arg(percent));
    }

    // Populate Browser Model
    populateBrowserModel(0);

    // Show Status Bar Message
    ui->statusBar->showMessage(QString("Updating Browser Data Model...done"));
}

//==============================================================================
// Clear Browser Model
//==============================================================================
void MainBrowserWindow::clearBrowserModel()
{
    // Show Status Bar Message
    ui->statusBar->showMessage(QString("Cleaning Up Browser Data Model..."));

    // Init Empty List
    QList<QObject*> emptyList;

    // Get QML Context
    QQmlContext* qmlContext = ui->browserWidget->rootContext();

    // Set Context Property - Browser Data Model
    qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(emptyList));

    // Get Browser Model Count
    int bmCount = browserDataModel.count();

    // Check Browser Data Model Count
    if (bmCount > 0) {
        qDebug() << "MainBrowserWindow::clearBrowserModel - bmCount: " << bmCount;

        // Go Thru Browser Data Model While Last Item
        while (!browserDataModel.isEmpty()) {
            // Get Last Item
            QObject* lastItem = browserDataModel.takeLast();
            // Delete Last Item
            delete lastItem;
        }
    }

    // Show Status Bar Message
    ui->statusBar->showMessage(QString("Cleaning Up Browser Data Model...done"));
}

//==============================================================================
// Populate Browser Model
//==============================================================================
void MainBrowserWindow::populateBrowserModel(const int& aIndex)
{
    qDebug() << "MainBrowserWindow::populateBrowserModel - aIndex: " << aIndex;

    // Get Browser Widget QML Context
    QQmlContext* qmlContext = ui->browserWidget->rootContext();
    // Init Empty List
    QList<QObject*> emptyList;

    // Set Context Property - Browser Data Model
    qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(emptyList));
    // Set Context Property - Browser Data Model
    qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(browserDataModel));

    // Check Viewer Window
    if (viewerWindow) {
        // Get Viewer Widget QML Context
        qmlContext = viewerWindow->getQMLContext();
        // Set Context Property - Browser Data Model
        qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(emptyList));
        // Set Context Property - Browser Data Model
        qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(browserDataModel));
    }

    // Set Current Index
    setCurrentIndex(aIndex);
}

//==============================================================================
// Rotate Current Image Left
//==============================================================================
void MainBrowserWindow::rotateLeft()
{
    qDebug() << "MainBrowserWindow::rotateLeft";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTRotateFilesLeft);
}

//==============================================================================
// Rotate Current Image Right
//==============================================================================
void MainBrowserWindow::rotateRigth()
{
    qDebug() << "MainBrowserWindow::rotateRigth";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTRotateFilesRight);
}

//==============================================================================
// Flip Current Image Horizontally
//==============================================================================
void MainBrowserWindow::flipHorizontally()
{
    qDebug() << "MainBrowserWindow::flipHorizontally";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTFlipFilesHorizontally);
}

//==============================================================================
// Flip Current Image Vertically
//==============================================================================
void MainBrowserWindow::flipVertically()
{
    qDebug() << "MainBrowserWindow::flipVertically";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTFlipFilesVertically);
}

//==============================================================================
// Delete Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::deleteFile()
{
    qDebug() << "MainBrowserWindow::deleteFile";

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTDeleteFiles);
}

//==============================================================================
// Delete Current/Selected File
//==============================================================================
void MainBrowserWindow::renameFile()
{
    qDebug() << "MainBrowserWindow::renameFile";

    // Init Generic Dialog
    QDialog* dialog = NULL;

    // Check If Model Has Selection
    if (hasSelection()) {

        // Check Rename Series Dialog
        if (!renameSeriesDialog) {
            // Create Rename Series Dialog
            renameSeriesDialog = new RenameSeriesDialog;
        }

        // Set Dialog
        dialog = renameSeriesDialog;

    } else {

        // Check REname File Dialog
        if (!renameFileDialog) {
            // Create Rename File Dialog
            renameFileDialog = new RenameFileDialog;
        }

        // Init index
        int index = popupIndex >= 0 ? popupIndex : currentIndex ;
        // Set File Name
        QString fileName = static_cast<BrowserDataObject*>(browserDataModel[index])->fileName;
        // Set File Name
        renameFileDialog->setFileName(fileName);

        // Set Dialog
        dialog = renameFileDialog;
    }

    // Check Dialog
    if (dialog && dialog->exec()) {

        // Evaluate File Rename Pattern if Has Selection
        if (hasSelection() && !evaluateFileRenamePattern()) {
            return;
        }

        // Do Rename File
        //doRenameFile();

        // Init Worker
        initWorker();

        // Emit Operate Worker Signal
        emit operateWorker(OTRenameFiles);
    }
}

//==============================================================================
// Copy To Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::copyToDirectory()
{
    qDebug() << "MainBrowserWindow::copyToDirectory";

    // Check Dir Selector
    if (!dirSelector) {
        // Create Dir Selector
        dirSelector = new DirSelectorDialog();
    }

    // Check Comapre Images Dialog
    if (!compareDialog) {
        // Create Compare Dialog
        compareDialog = new CompareDialog();
    }

    // Check Dir Selector
    if (dirSelector) {
        // Set Title
        dirSelector->setWindowTitle(QString("Copy To Directory..."));

        // Exec Dir Selector Dialog
        if (dirSelector->exec()) {
            // Init Worker
            initWorker();

            // Emit Operate Worker Signal
            emit operateWorker(OTCopyToFiles);
        }
    }
}

//==============================================================================
// Move To Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::moveToDirectory()
{
    qDebug() << "MainBrowserWindow::moveToDirectory";

    // Check Dir Selector
    if (!dirSelector) {
        // Create Dir Selector
        dirSelector = new DirSelectorDialog();
    }

    // Check Dir Selector
    if (dirSelector) {
        // Set Title
        dirSelector->setWindowTitle(QString("Move To Directory..."));

        // Exec Dir Selector Dialog
        if (dirSelector->exec()) {
            // Do Move To Directory
            //doMoveToDirectory();

            // Init Worker
            initWorker();

            // Emit Operate Worker Signal
            emit operateWorker(OTMoveToFiles);
        }
    }
}

//==============================================================================
// Rotate Current/Selected Image(s) Left
//==============================================================================
void MainBrowserWindow::doRotateLeft()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {

        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();

        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;

        // Got Thru Model
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Rotate By Index
                rotateFileByIndex(i, RDTLeft);
                // Inc Selected Index
                selectedIndex++;
                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

    } else {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doRotateLeft - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doRotateLeft - currentIndex: " << currentIndex;
        }

        // Rotate By Index
        rotateFileByIndex(index, RDTLeft);
    }
}

//==============================================================================
// Rotate Current/Selected Image(s) Right
//==============================================================================
void MainBrowserWindow::doRotateRigth()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {

        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();
        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;

        // Got Thru Model
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Rotate By Index
                rotateFileByIndex(i, RDTRight);
                // Inc Selected Index
                selectedIndex++;
                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

    } else {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doRotateRigth - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doRotateRigth - currentIndex: " << currentIndex;
        }

        // Rotate By Index
        rotateFileByIndex(index, RDTRight);
    }
}

//==============================================================================
// Flip Current/Selected Image(s) Horizontally
//==============================================================================
void MainBrowserWindow::doFlipHorizontally()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {

        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();
        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;

        // Got Thru Model
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Flip By Index
                flipFileByIndex(i, FDTHorizontal);
                // Inc Selected Index
                selectedIndex++;
                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

    } else {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doFlipHorizontally - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doFlipHorizontally - currentIndex: " << currentIndex;
        }

        // Flip By Index
        flipFileByIndex(index, FDTHorizontal);
    }
}

//==============================================================================
// Flip Current/Selected Image(s) Vertically
//==============================================================================
void MainBrowserWindow::doFlipVertically()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {

        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();
        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;

        // Got Thru Model
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Flip By Index
                flipFileByIndex(i, FDTVertical);
                // Inc Selected Index
                selectedIndex++;
                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

    } else {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doFlipVertically - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doFlipVertically - currentIndex: " << currentIndex;
        }

        // Flip By Index
        flipFileByIndex(index, FDTVertical);
    }
}

//==============================================================================
// Delete Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::doDeleteFile()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {
        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();
        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;
        // Init Last Index
        int lastIndex = -1;

        // Got Thru Model
        for (int i=bdmCount-1; i>=0; i--) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Set/Save Last Index
                lastIndex = i;
                // Delete File By Index
                deleteFileByIndex(i, false, false);
                // Inc Selected Index
                selectedIndex++;
                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

        // Check Worker
        if (worker) {
            // Emit Populate Browser Data Model
            emit worker->populateBrowserModel(lastIndex);
        } else {
            // Populate Browser Data Model
            populateBrowserModel(lastIndex);
        }

        // Get Deleted Items Count
        while (deletedItems.count() > 0) {
            // Get Last Item
            QObject* lastItem = deletedItems.takeLast();
            // Delete Last Item
            lastItem->deleteLater();
        }

    } else if (!deleteSelectedOnly) {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doDeleteFile - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doDeleteFile - currentIndex: " << currentIndex;
        }

        // Delete File By Index
        deleteFileByIndex(index);
    }
}

//==============================================================================
// Rename Current/Selected File
//==============================================================================
void MainBrowserWindow::doRenameFile()
{
    // Check If Browser Grid Has Selections
    if (hasSelection()) {
        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();
        // Get Selected Item Count
        int selectedCount = numberOfSelected();
        // Init Selected Index
        int selectedIndex = 0;

        // Get Start Index
        int patternIndex = renameSeriesDialog->getStartIndex();
        // Get Pattern Text
        QString patternText = renameSeriesDialog->getPattern();

        // Get # Digit Count
        int digitCount = patternText.count("#");

        // Get Hash Index
        int hashIndex = patternText.indexOf("#");

        // Init Target Template
        QString targetTemplate = patternText.replace(hashIndex, digitCount, QString("%1"));

        // Got Thru Model
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Check If Selected
            if (item->fileSelected) {
                // Init File Info
                QFileInfo fileInfo(currentDir + QString("/") + item->fileName);

                // Rename File By Index
                renameFileByIndex(i, targetTemplate.arg(patternIndex, digitCount, 10, QChar('0')) + QString(".") + fileInfo.suffix(), false);

                // Inc Pattern Index
                patternIndex++;

                // Inc Selected Index
                selectedIndex++;

                // Emit Progress Changed Signal
                emit progressChanged(selectedCount, selectedIndex);
            }
        }

        // Check Worker
        if (worker) {
            // Emit Refresh View Signal
            emit worker->refreshView();
        } else {
            // Call refresh View Slot
            refreshView();
        }

    } else {
        // Init index
        int index = currentIndex ;

        // Check Popup Index
        if (popupIndex != -1) {
            qDebug() << "MainBrowserWindow::doRenameFile - popupIndex: " << popupIndex;

            // Set Index
            index = popupIndex;
            // Reset Popup Index
            popupIndex = -1;

        } else {
            qDebug() << "MainBrowserWindow::doRenameFile - currentIndex: " << currentIndex;
        }

        // Rename File By Index
        renameFileByIndex(index, renameFileDialog->getFileName());
    }
}

//==============================================================================
// Copy Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::doCopyToDirectory()
{
    // Check Dir Selector & Selected Dir
    if (dirSelector && dirSelector->getSelectedDir() != currentDir) {
        qDebug() << "MainBrowserWindow::doCopyToDirectory - selectedDir: " << dirSelector->getSelectedDir();

        // Check If Browser Grid Has Selections
        if (hasSelection()) {
            // Get Browser Data Model Count
            int bdmCount = browserDataModel.count();
            // Get Selected Item Count
            int selectedCount = numberOfSelected();
            // Init Selected Index
            int selectedIndex = 0;

            // Got Thru Model
            for (int i=0; i<bdmCount; i++) {
                // Get Item
                BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
                // Check If Selected
                if (item->fileSelected) {
                    // Reset File Selected
                    //item->fileSelected = false;
                    // Copy By Index
                    copyFileByIndex(i, dirSelector->getSelectedDir());
                    // Inc Selected Index
                    selectedIndex++;
                    // Emit Progress Changed Signal
                    emit progressChanged(selectedCount, selectedIndex);
                }
            }

        } else {
            // Init index
            int index = currentIndex ;

            // Check Popup Index
            if (popupIndex != -1) {
                qDebug() << "MainBrowserWindow::doCopyToDirectory - popupIndex: " << popupIndex;

                // Set Index
                index = popupIndex;
                // Reset Popup Index
                popupIndex = -1;

            } else {
                qDebug() << "MainBrowserWindow::doCopyToDirectory - currentIndex: " << currentIndex;
            }

            // Copy To By Index
            copyFileByIndex(index, dirSelector->getSelectedDir());

        }
    } else {
        qDebug() << "MainBrowserWindow::doCopyToDirectory - Target Dir is the same as current...";
    }
}

//==============================================================================
// Move Current/Selected File(s)
//==============================================================================
void MainBrowserWindow::doMoveToDirectory()
{
    // Check Dir Selector & Selected Dir
    if (dirSelector && dirSelector->getSelectedDir() != currentDir) {
        qDebug() << "MainBrowserWindow::doMoveToDirectory - selectedDir: " << dirSelector->getSelectedDir();

        // Check If Browser Grid Has Selections
        if (hasSelection()) {

            // Get Browser Data Model Count
            int bdmCount = browserDataModel.count();
            // Get Selected Item Count
            int selectedCount = numberOfSelected();
            // Init Selected Index
            int selectedIndex = 0;
            // Init Last Index
            int lastIndex = -1;

            // Got Thru Model
            for (int i=bdmCount-1; i>=0; i--) {
                // Get Item
                BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

                // Check If Selected
                if (item->fileSelected) {
                    // Set Last Index
                    lastIndex = i;
                    // Move By Index
                    moveFileByIndex(i, dirSelector->getSelectedDir(), false, false);
                    //moveFileByIndex(i, dirSelector->getSelectedDir());
                    // Inc Selected Index
                    selectedIndex++;
                    // Emit Progress Changed Signal
                    emit progressChanged(selectedCount, selectedIndex);
                }
            }

            // Check Worker
            if (worker) {
                // Emit Populat Browser Model
                emit worker->populateBrowserModel(lastIndex);
            } else {
                // Populate Browser Data Model
                populateBrowserModel(lastIndex);
            }
/*
            // Check Last Index
            if (lastIndex != -1) {
                // Set Current Index
                setCurrentIndex(lastIndex);
            }
*/
            // Get Deleted Items Count
            while (deletedItems.count() > 0) {
                // Get Last Item
                QObject* lastItem = deletedItems.takeLast();
                // Delete Last Item
                lastItem->deleteLater();
            }

        } else {
            // Init index
            int index = currentIndex ;

            // Check Popup Index
            if (popupIndex != -1) {
                qDebug() << "MainBrowserWindow::doMoveToDirectory - popupIndex: " << popupIndex;

                // Set Index
                index = popupIndex;
                // Reset Popup Index
                popupIndex = -1;

            } else {
                qDebug() << "MainBrowserWindow::doMoveToDirectory - currentIndex: " << currentIndex;
            }

            // Move By Index
            moveFileByIndex(index, dirSelector->getSelectedDir());

        }
    } else {
        qDebug() << "MainBrowserWindow::doMoveToDirectory - Target Dir is the same as current...";
    }
}

//==============================================================================
// Find Duplicate Files
//==============================================================================
void MainBrowserWindow::doFindDuplicates()
{
    // Check Duplicate List Model
    if (!duplicateListModel) {
        qDebug() << "MainBrowserWindow::doFindDuplicates - NO DUPLICATE MODEL!!";

        return;
    }

    qDebug() << "MainBrowserWindow::doFindDuplicates";

    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Clear Duplicate List Model
    duplicateListModel->clear();

    // Check If Browser Grid Has Selections
    if (hasSelection()) {

    } else {
        // Go Thru Model
        for (int i=0; i<bdmCount; ++i) {
            // Get Model Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
            // Init File Info
            QFileInfo fileInfo(currentDir + "/" + item->fileName);

            // Go Thru Rest Of The Model
            for (int j=i+1; j<bdmCount; ++j) {
                // Get Model Item
                BrowserDataObject* otherItem = static_cast<BrowserDataObject*>(browserDataModel[j]);
                // Init Other File Info
                QFileInfo otherFileInfo(currentDir + "/" + otherItem->fileName);

                // Compare Size
                if (fileInfo.size() == otherFileInfo.size()) {

                    // Init Image
                    QImage fileImage(fileInfo.absoluteFilePath());
                    // Init Other Image
                    QImage otherFileImage(otherFileInfo.absoluteFilePath());

                    // Compare Images
                    if (duplicateListModel->indexOfValue(otherFileInfo.absoluteFilePath()) < 0 && compareImagesByPixel(fileImage, otherFileImage)) {

                        // Check File Path If Already Added
                        if (duplicateListModel->indexOfValue(fileInfo.absoluteFilePath()) < 0) {
                            // Insert File Path
                            duplicateListModel->addData(i, fileInfo.absoluteFilePath());
                        }

                        // Insert Other File Path
                        duplicateListModel->addData(i, otherFileInfo.absoluteFilePath() + QString(DUPLICATE_FILE_LIST_ITEM_SELECTED_SUFFIX));

                        //qDebug() << "MainBrowserWindow::doFindDuplicates - count: " << duplicateListModel->rowCount() << " - fileName: " << otherFileInfo.fileName();

                    }
                }
            }

            // Emit Progress Changed Signal
            emit progressChanged(bdmCount, i);
        }
    }
}

//==============================================================================
// Start Slide Show Timer
//==============================================================================
void MainBrowserWindow::startSlideShow()
{
    // Check Slide Show Timer ID
    if (slideShowTimerID == -1) {
        qDebug() << "MainBrowserWindow::startSlideShow";

        // Init Settings
        QSettings settings;
        // Start Timer
        slideShowTimerID = startTimer(settings.value(SETTINGS_KEY_SLIDE_SHOW_DELAY).toInt());
    }
}

//==============================================================================
// Stop Slide Show Timer
//==============================================================================
void MainBrowserWindow::stopSlideShow()
{
    // Check Timer ID
    if (slideShowTimerID != -1) {
        qDebug() << "MainBrowserWindow::stopSlideShow";

        // Kill Timer
        killTimer(slideShowTimerID);
        // Reset Slide Show Timer ID
        slideShowTimerID = -1;
    }
}

//==============================================================================
// Register App For Supported Images
//==============================================================================
void MainBrowserWindow::registerAppForImages()
{
    qDebug() << "MainBrowserWindow::registerAppForImages";

/*
    defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>jpg</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
    defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>jpeg</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
    defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>gif</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
    defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>png</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"
    defaults write com.apple.LaunchServices LSHandlers -array-add "<dict><key>LSHandlerContentTag</key><string>bmp</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>com.yourcompany.MaxViewer</string></dict>"

    /System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister -kill -domain local-domain system -domain user
*/

    // Init Defaults Command %1 is extension eg.: jpg, %2 is app name eg.: com.yourcompany.MaxViewer
    QString defaultsCommand = QString("defaults write com.apple.LaunchServices LSHandlers -array-add \"<dict><key>LSHandlerContentTag</key><string>%1</string><key>LSHandlerContentTagClass</key><string>public.filename-extension</string><key>LSHandlerRoleAll</key><string>%2</string></dict>\"");

    // Init lsregister Command
    QString lsregisterCommand = QString("/System/Library/Frameworks/CoreServices.framework/Versions/A/Frameworks/LaunchServices.framework/Versions/A/Support/lsregister -kill -domain local-domain system -domain user");

    // ...

    // Init Result
    int result = 0;

    // Exec Register File Types
    result = system(defaultsCommand.arg("jpg").arg("com.yourcompany.MaxViewer").toStdString().c_str());
    result = system(defaultsCommand.arg("jpeg").arg("com.yourcompany.MaxViewer").toStdString().c_str());
    result = system(defaultsCommand.arg("png").arg("com.yourcompany.MaxViewer").toStdString().c_str());
    result = system(defaultsCommand.arg("gif").arg("com.yourcompany.MaxViewer").toStdString().c_str());
    result = system(defaultsCommand.arg("bmp").arg("com.yourcompany.MaxViewer").toStdString().c_str());

    // ...

    // Restart Finder Service
    result = system(lsregisterCommand.toStdString().c_str());

    // ...
}

//==============================================================================
// Compare Images
//==============================================================================
void MainBrowserWindow::compareImages()
{
    qDebug() << "MainBrowserWindow::compareImages";

    // Check Comapre Images Dialog
    if (!compareDialog) {
        // Create Dialog
        compareDialog = new CompareDialog();
    }

    // Init Left Image
    QString leftImage = QString("");
    // Init Right Image
    QString rightImage = QString("");
    // Init Title Left
    QString titleLeft = QString("");
    // Init title Right
    QString titleRight = QString("");
    // Init Item
    BrowserDataObject* item = NULL;

    // Check If Has Selection
    if (hasSelection()) {
        // Get Browser Data Model Count
        int bdmCount = browserDataModel.count();

        // Go Thru Items
        for (int i=0; i<bdmCount; i++) {
            // Get Item
            item = static_cast<BrowserDataObject*>(browserDataModel[i]);

            // Check If Item Selected
            if (item->fileSelected) {
                // Check Left Image
                if (leftImage.isEmpty()) {
                    // Set Left Image
                    leftImage = currentDir + QString("/") + item->fileName;
                    // Set Left Title
                    titleLeft = item->fileName;
                } else {
                    // Check Right Image
                    if (rightImage.isEmpty()) {
                        // Set Right Image
                        rightImage = currentDir + QString("/") + item->fileName;
                        // Set Right Title
                        titleRight = item->fileName;
                    } else {
                        break;
                    }
                }
            }
        }
    } else {
        // Get Item
        item = static_cast<BrowserDataObject*>(browserDataModel[currentIndex]);
        // Get Current File Name
        leftImage = currentDir + QString("/") + item->fileName;
        // Set Left Title
        titleLeft = item->fileName;
        // Check Popup Index
        if (popupIndex != -1) {
            // Get Item
            item = static_cast<BrowserDataObject*>(browserDataModel[popupIndex]);
            // Set Right Title
            titleRight = item->fileName;
            // Set Right Image
            rightImage = currentDir + QString("/") + item->fileName;
            // Reset Popup Index
            popupIndex = -1;
        }
    }

    // Set Current Dir
    compareDialog->setCurrentDir(currentDir);

    // Set Left Image File
    compareDialog->setLeftImage(leftImage);
    // Set Right Image File
    compareDialog->setRightImage(rightImage);

    // Set Window Title
    compareDialog->setWindowTitle(QString("Compare Images: %1 <-> %2").arg(titleLeft).arg(titleRight));

    // ...

    // Configure Buttons
    compareDialog->configureButtons(QDialogButtonBox::Close);

    // Exec
    compareDialog->exec();

    // ...
}

//==============================================================================
// Find Duplicates in the Current Directory
//==============================================================================
void MainBrowserWindow::findDuplicates()
{
    // Select None
    selectNone();

    qDebug() << "MainBrowserWindow::findDuplicates";

    // Check Duplicate List Model
    if (!duplicateListModel) {
        // Create Duplicate List Model
        duplicateListModel = new DuplicateListModel();
    }

    // Init Worker
    initWorker();

    // Emit Operate Worker Signal
    emit operateWorker(OTFindDuplicates);
}

//==============================================================================
// Find Model Index By File Name
//==============================================================================
int MainBrowserWindow::findIndex(const QString& aFileName)
{
    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Go Thru Model
    for (int i=0; i<bdmCount; ++i) {
        // Get Model Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);
        // Check File Name
        if (aFileName == item->fileName) {
            return i;
        }
    }

    return -1;
}

//==============================================================================
// Refresh View
//==============================================================================
void MainBrowserWindow::refreshView(const int& aIndex)
{
    qDebug() << "MainBrowserWindow::refreshView";

    // Get Current Index
    int lastIndex = aIndex != -1 ? aIndex : currentIndex;

    // Clear Browser Model
    clearBrowserModel();

    // Update Browser Model
    updateBrowserModel();

    // Populate Browser Model
    populateBrowserModel(lastIndex);

    // ...
}

//==============================================================================
// Set Sorting Type
//==============================================================================
void MainBrowserWindow::setSortType(const int& aSortType)
{
    qDebug() << "MainBrowserWindow::setSortType - aSortType: " << aSortType;

    // Check Sort Type
    if (sortType != aSortType) {
        // Init Settings
        QSettings settings;
        // Set Value
        settings.setValue(SETTINGS_KEY_SLIDE_SORT_TYPE, aSortType);
        // Set Value
        settings.setValue(SETTINGS_KEY_SLIDE_SORT_ORDER, false);
        // Refresh View
        refreshView();
    }
}

//==============================================================================
// Set Sort Reverse Order
//==============================================================================
void MainBrowserWindow::setReverseOrder(const bool& aReverse)
{
    qDebug() << "MainBrowserWindow::setReverseOrder - aReverse: " << aReverse;

    // Check Reverse Order
    if (reverseOrder != aReverse) {
        // Init Settings
        QSettings settings;
        // Set Value
        settings.setValue(SETTINGS_KEY_SLIDE_SORT_ORDER, aReverse);
        // Refresh View
        refreshView();
    }

}

//==============================================================================
// Delete File By Index
//==============================================================================
void MainBrowserWindow::deleteFileByIndex(const int& aIndex, const bool& aDeleteItem, const bool& aNotify)
{
    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Init File Name
        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init File Info
        QFileInfo fileInfo(fileName);

        // Check File Info - Check Settings
        if (fileInfo.exists()) {

            qDebug() << "MainBrowserWindow::deleteFileByIndex - aIndex: " << aIndex;

            // Init Dir
            QDir dir(fileInfo.canonicalPath());

            // Delete File
            if (!dir.remove(fileName)) {
                qDebug() << "MainBrowserWindow::deleteFileByIndex - aIndex: " << fileName << " - ERROR!";
            }

            // Get Item
            BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel.takeAt(aIndex));

            // Check Notify
            if (aNotify) {
                // Emit Deleting Index Signal
                emit deletingIndex(aIndex);

                // Check Worker
                if (worker) {
                    // Emit Populate Browser Data Model
                    emit worker->populateBrowserModel(aIndex);
                } else {
                    // Populate Browser Data Model
                    populateBrowserModel(aIndex);
                }

                // Emit Index Removed Signal
                emit indexDeleted(aIndex);

                // Set Current Index
                //setCurrentIndex(aIndex);
            }

            // Check Delete Item
            if (aDeleteItem) {
                // Delte Item - Have to do it last
                //delete item;
                item->deleteLater();
            } else {
                // Append To Deleted Item List
                deletedItems << item;
            }
        }
    }
}

//==============================================================================
// Rotate File By Index
//==============================================================================
void MainBrowserWindow::rotateFileByIndex(const int& aIndex, int aDirection, const bool& aNotify)
{
    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Init File Name
        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Image
        QImage image(fileName);

        // Init Transform
        QTransform transform;
        // Set Rotation
        transform.rotate(aDirection == RDTLeft ? TRANSFORM_ROTATE_LEFT : TRANSFORM_ROTATE_RIGHT);

        // Get Transformed Image
        image = image.transformed(transform);

        // Save Image
        image.save(fileName);

        // For Refresh

        // Check Notify
        if (aNotify) {
            // Emit File Updated Signal
            emit fileUpdated(aIndex);
        }
    }
}

//==============================================================================
// Flip File By Index
//==============================================================================
void MainBrowserWindow::flipFileByIndex(const int& aIndex, int aDirection, const bool& aNotify)
{
    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Init File Name
        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Image
        QImage image(fileName);
        // Get Mirrored Image
        image = image.mirrored(aDirection == FDTHorizontal, aDirection == FDTVertical);

        // Save Image
        image.save(fileName);

        // For Refresh

        // Check Notify
        if (aNotify) {
            // Emit File Updated Signal
            emit fileUpdated(aIndex);
        }
    }
}

//==============================================================================
// Copy File By Index
//==============================================================================
void MainBrowserWindow::copyFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aNotify)
{
    Q_UNUSED(aNotify);

    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Init File Name
        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Target File Name
        QString targetFileName(aTargetDir);
        // Add Separator
        targetFileName += "/";
        // Add Current File Name
        targetFileName += static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Current File
        QFile file(fileName);
        // Init Target File
        QFile targetFile(targetFileName);

        // Check Target File - TODO: Check Settings

        // Check If Target File Exists
        if (targetFile.exists()) {
            //qDebug("MainBrowserWindow::copyFileByIndex - Overwriting...");

            // Show Image Compare Dialog - Check Settings

            // Delete Target File
            if (!targetFile.remove()) {
                qDebug("MainBrowserWindow::copyFileByIndex - ERROR CLEARING TARGET FILE!");

                return;
            }
        }

        // Copy File(s)
        if (!file.copy(targetFileName)) {
            qDebug("MainBrowserWindow::copyFileByIndex - ERROR COPY FILE!");
        }
    }
}

//==============================================================================
// Move File By Index
//==============================================================================
void MainBrowserWindow::moveFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aDeleteItem, const bool& aNotify)
{
    Q_UNUSED(aNotify);

    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Init File Name
        QString fileName = currentDir + QString("/") + static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Target File Name
        QString targetFileName(aTargetDir);
        // Add Separator
        targetFileName += "/";
        // Add Current File Name
        targetFileName += static_cast<BrowserDataObject*>(browserDataModel[aIndex])->fileName;

        // Init Current File
        QFile file(fileName);
        // Init Target File
        QFile targetFile(targetFileName);

        // Check Target File - TODO: Check Settings
        if (targetFile.exists()) {
            qDebug("MainBrowserWindow::moveFileByIndex - Overwriting...");

            // Show Image Compare Dialog - Check Settings

            // Delete Target File
            if (!targetFile.remove()) {
                qDebug("MainBrowserWindow::moveFileByIndex - ERROR CLEARING TARGET FILE!");

                return;
            }
        }

        // Rename/Move File(s)
        if (!file.rename(targetFileName)) {
            qDebug("MainBrowserWindow::moveFileByIndex - ERROR COPY FILE!");
        }

        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel.takeAt(aIndex));

        // Check Notify
        if (aNotify) {
            // Emit Deleting Index Signal
            emit deletingIndex(aIndex);

            // Check Worker
            if (worker) {
                // Emit Populate Browser Data Model
                emit worker->populateBrowserModel(aIndex);
            } else {
                // Populate Browser Data Model
                populateBrowserModel(aIndex);
            }

            // Emit Index Removed Signal
            emit indexDeleted(aIndex);

            // Set Current Index
            //setCurrentIndex(aIndex);
        }

        if (aDeleteItem) {
            // Delte Item - Have to do it last
            //delete item;
            item->deleteLater();
        } else {
            // Append To Deleted Items
            deletedItems << item;

            qDebug() << "MainBrowserWindow::moveFileByIndex - added deleted item";
        }
    }
}

//==============================================================================
// Rename File By Index
//==============================================================================
void MainBrowserWindow::renameFileByIndex(const int& aIndex, const QString& aFileName, const bool& aNotify)
{
    // Get Model Count
    int bdmCount = browserDataModel.count();

    // Check Index
    if (aIndex >=0 && aIndex < bdmCount) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[aIndex]);

        // Init Full File Name
        QString fileName = currentDir + QString("/") + item->fileName;

        // Init Target File Name
        QString targetFileName(currentDir);
        // Add Separator
        targetFileName += "/";
        // Add Target File Name
        targetFileName += aFileName;

        // Init Current File
        QFile file(fileName);
        // Init Target File
        QFile targetFile(targetFileName);

        // Check Target File - TODO: Check Settings
        if (targetFile.exists()) {
            qDebug("MainBrowserWindow::renameFileByIndex - Overwriting...");

            // Show Image Compare Dialog - Check Settings

            // Delete Target File
            if (!targetFile.remove()) {
                qDebug("MainBrowserWindow::renameFileByIndex - ERROR CLEARING TARGET FILE!");

                return;
            }
        }

        // Rename File
        if (file.rename(targetFileName)) {
            // Set Browser Data Model File Name
            item->setFileName(aFileName);

            // Check Index
            if (aIndex == currentIndex) {
                // Update Current File
                setCurrentFile(targetFileName);
            }

        } else {
            qDebug("MainBrowserWindow::renameFileByIndex - ERROR RENAME FILE!");
        }

        // Check Notify
        if (aNotify) {
            // Emit File Updated Signal
            emit fileUpdated(aIndex);

            // Check Worker
            if (worker) {
                // Emit File Renamed Signal
                emit worker->fileRenamed(aFileName);
            } else {
                // Call File Renamed Slot
                fileRenamed(aFileName);
            }
        }
    }
}

//==============================================================================
// Evaluate File Rename Pattern
//==============================================================================
bool MainBrowserWindow::evaluateFileRenamePattern()
{
    // Check REname Series Dialog
    if (!renameSeriesDialog) {
        return false;
    }

    // Get Pattern
    QString pattern = renameSeriesDialog->getPattern();

    // Check Pattern
    if (pattern.isEmpty()) {

        // Check Info Dialog
        if (!infoDialog) {
            // Create Info Dialog
            infoDialog = new InfoDialog;
        }

        // Show Invalid Pattern Dialog
        infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

        return false;
    }

    // Get Index Of #
    int hashIndex = pattern.indexOf("#");

    // Check Hash Index
    if (hashIndex < 0) {
        // Check Info Dialog
        if (!infoDialog) {
            // Create Info Dialog
            infoDialog = new InfoDialog;
        }

        // Show Invalid Pattern Dialog
        infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

        return false;
    }

    // Set Prev Hash Index
    int prevHashIndex = hashIndex;

    // Go Thru Pattern Checking Hash Indexes
    while (hashIndex >= 0) {
        // Get Next Hash Index
        hashIndex = pattern.indexOf("#", prevHashIndex + 1);

        // Check Hash Index
        if (hashIndex - prevHashIndex > 1) {
            // Check Info Dialog
            if (!infoDialog) {
                // Create Info Dialog
                infoDialog = new InfoDialog;
            }

            // Show Invalid Pattern Dialog
            infoDialog->showDialogWithText(QString("Invalid File Name Pattern!"));

            return false;
        }

        // Set Prev Hash Index
        prevHashIndex = hashIndex;
    }

    // ...

    return true;
}

//==============================================================================
// Launch Viewer
//==============================================================================
void MainBrowserWindow::launchViewer(const QString& aFilePath)
{
    // Check Viewer Window
    if (!viewerWindow) {
        // Get Instance
        viewerWindow = MainViewerWindow::getInstance();
    }

    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Check File Path
    if (!aFilePath.isEmpty() && fileInfo.exists()) {
        qDebug() << "MainBrowserWindow::launchViewer - aFilePath: " << aFilePath;

        // Extract File Path
        QString fileDir = fileInfo.canonicalPath();

        // Set Current Dir / Update Model
        setCurrentDir(fileDir);

        // Find File Index
        int index = findIndex(fileInfo.fileName());

        // Set Current Index
        setCurrentIndex(index);

        // Hide
        hide();

        // Get Viewer Widget QML Context
        QQmlContext* qmlContext = viewerWindow->getQMLContext();
        // Set Main View Controller
        qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);
        // Set Viewer Controller
        qmlContext->setContextProperty(VIEWER_VIEW_CONTROLLER, viewerWindow);
        // Set Context Property - Browser Data Model
        qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(browserDataModel));

        // Show Viewer Window Full Screen
        viewerWindow->showFullScreen();
        // Show Status Text
        viewerWindow->showStatusText(aFilePath, DEFAULT_STATUS_TEXT_SHOW_TIMEOUT);

        // ...

    } else {
        qDebug() << "MainBrowserWindow::launchViewer";

        // Init Settings
        QSettings settings;

        // Check Full Screen
        if (settings.value(SETTINGS_KEY_VIEWER_FULLSCREEN).toBool()) {
            // Hide Current Browser Window
            hide();
        }

        // Get Viewer Widget QML Context
        QQmlContext* qmlContext = viewerWindow->getQMLContext();
        // Set Main View Controller
        qmlContext->setContextProperty(MAIN_VIEW_CONTROLLER, this);
        // Set Viewer Controller
        qmlContext->setContextProperty(VIEWER_VIEW_CONTROLLER, viewerWindow);
        // Set Context Property - Browser Data Model
        qmlContext->setContextProperty(MAIN_VIEW_DATA_MODEL, QVariant::fromValue(browserDataModel));

        // Show Viewer Window
        viewerWindow->showWindow();
        // Show Status Text
        viewerWindow->showStatusText(currentFile, DEFAULT_STATUS_TEXT_SHOW_TIMEOUT);

        // ...
    }
}

//==============================================================================
// Hide Viewer
//==============================================================================
void MainBrowserWindow::hideViewer()
{
    // Check Viewer Window
    if (viewerWindow) {
        qDebug() << "MainBrowserWindow::hideViewer";

        // Hide Viewer Window
        viewerWindow->hideWindow();

        // Stop Slide Show
        stopSlideShow();
    }

    // Show Current Borwser Window
    show();

    // ...
}

//==============================================================================
// Toggle Viewer Fullscreen
//==============================================================================
void MainBrowserWindow::toggleViewerFullScreen()
{
    // Check Viewer Window
    if (viewerWindow) {
        // Check Full Screen Status
        if (viewerWindow->windowState() == Qt::WindowFullScreen) {

            // Show
            viewerWindow->showNormal();
            // Save Settings
            viewerWindow->saveSettings();
            // Show Browser Window
            show();
            // Raise Window
            viewerWindow->raise();
            // Activate Window To Regain Focus
            viewerWindow->activateWindow();

        } else {
            // Show
            viewerWindow->showFullScreen();
            // Save Settings
            viewerWindow->saveSettings();
            // Hide
            hide();
        }
    }
}

//==============================================================================
// Launch Preferences
//==============================================================================
void MainBrowserWindow::launchPreferences()
{
    qDebug() << "MainBrowserWindow::launchPreferences";

    // Init Preferences Dialog
    PreferencesDialog pDialog;

    // Launch Dialog
    pDialog.launchDialog();

    // ...
}

//==============================================================================
// Launch About Dialog
//==============================================================================
void MainBrowserWindow::launchAboutDialog()
{
    qDebug() << "MainBrowserWindow::launchPreferences";

    // Check About Dialog
    if (!aboutDialog) {
        // Create About Dialog
        aboutDialog = new AboutForm();
    }

    // Launch Dialog
    aboutDialog->show();

    // ...
}

//==============================================================================
// Toggle Slide Show
//==============================================================================
void MainBrowserWindow::toggleSlideShow()
{
    qDebug() << "MainBrowserWindow::toggleSlideShow";

    // Check Slide Show Timer ID
    if (slideShowTimerID == -1) {
        // Show Viewer
        launchViewer();
        // Start Slide Show
        startSlideShow();
    } else {
        // Stop Slide Show
        stopSlideShow();
    }
}

//==============================================================================
// Show Window
//==============================================================================
void MainBrowserWindow::showWindow()
{
    qDebug() << "MainBrowserWindow::showWindow";

    // Init Settings
    QSettings settings;

    // Get MaximizedState
    bool maximizedState = settings.value(SETTINGS_KEY_BROWSER_MAXIMIZED).toBool();

    // Check MAximized State
    if (maximizedState) {
        // Show Maximized
        showMaximized();
    } else {
        // Show
        show();
    }

    // Init Root Model Index
    QModelIndex modelIndex = fsModel->index(currentDir);

    // Expand
    ui->fileSystemTreeView->expand(modelIndex);
    // Set Current Index
    ui->fileSystemTreeView->setCurrentIndex(modelIndex);
}

//==============================================================================
// Hide Window
//==============================================================================
void MainBrowserWindow::hideWindow()
{
    qDebug() << "MainBrowserWindow::hideWindow";

    // Save Settings
    saveSettings();

    // Hide
    hide();
}

//==============================================================================
// Next Image
//==============================================================================
void MainBrowserWindow::nextImage()
{
    // Check Current Index
    if (currentIndex < browserDataModel.count()-1) {
        // Inc Current Index
        setCurrentIndex(currentIndex + 1);
    }
}

//==============================================================================
// Prev Image
//==============================================================================
void MainBrowserWindow::prevImage()
{
    // Check Current Index
    if (currentIndex > 0) {
        // Dec Current Index
        setCurrentIndex(currentIndex - 1);
    }
}

//==============================================================================
// Change Dir Up
//==============================================================================
void MainBrowserWindow::cdUp()
{
    qDebug() << "";

    // ...
}

//==============================================================================
// Get File Size In String Format
//==============================================================================
QString MainBrowserWindow::getFileSize(const QString& aFilePath)
{
    // Init File Info
    QFileInfo fileInfo(aFilePath);

    // Check If File Exists
    if (fileInfo.exists()) {

        // ...

        // Return Localized String
        return QString("%L1KB").arg(fileInfo.size() / 1024);
    }

    return QString("");
}

//==============================================================================
// Show Status Text
//==============================================================================
void MainBrowserWindow::showStatusText(const QString& aStatusText, const int& aTimeout)
{
    // Show Message
    ui->statusBar->showMessage(aStatusText, aTimeout);
}

//==============================================================================
// Handle File Open Event Slot
//==============================================================================
void MainBrowserWindow::handleFileOpenEvent(const QString& aFileName)
{
    qDebug() << "MainBrowserWindow::handleFileOpenEvent - aFileName: " << aFileName;

    // Show Status Text
    //showStatusText(aFileName);

    // Launch Viewer
    launchViewer(aFileName);

    // ...
}

//==============================================================================
// Select All Files in Current Dir
//==============================================================================
void MainBrowserWindow::selectAll()
{
    qDebug() << "MainBrowserWindow::selectAll";

    // Get Browser Data Model Count
    int bdmCount = browserDataModel.count();

    // Go Thru Items
    for (int i=0; i<bdmCount; i++) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

        // Check Item
        if (item) {
            // Set Selected
            item->setFileSelected(true);
        }
    }
}

//==============================================================================
// Select None Files in Current Dir
//==============================================================================
void MainBrowserWindow::selectNone()
{
    qDebug() << "MainBrowserWindow::selectNone";

    // Get Browser Data Model Count
    int bdmCount = browserDataModel.count();

    // Go Thru Items
    for (int i=0; i<bdmCount; i++) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

        // Check Item
        if (item) {
            // Set Selected
            item->setFileSelected(false);
        }
    }
}

//==============================================================================
// Set File Selected By Index
//==============================================================================
void MainBrowserWindow::setFileSelected(const int& aIndex, const bool& aSelected)
{
    qDebug() << "MainBrowserWindow::setFileSelected - aIndex: " << aIndex << " - aSelected: " << aSelected;

    // Get Browser Data Model Count
    int bdmCount = browserDataModel.count();
    // Check Inde
    if (aIndex >=0 && aIndex < bdmCount) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[aIndex]);

        // Check Item
        if (item) {
            // Set Selected
            item->setFileSelected(aSelected);
        }
    }
}

//==============================================================================
// Has Selection
//==============================================================================
bool MainBrowserWindow::hasSelection()
{
    // Get Browser Data Model Count
    int bdmCount = browserDataModel.count();

    // Go Thru Items
    for (int i=0; i<bdmCount; i++) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

        // Check If File Selected
        if (item && item->fileSelected) {
            return true;
        }
    }

    return false;
}

//==============================================================================
// Get Number Of Selected Items
//==============================================================================
int MainBrowserWindow::numberOfSelected()
{
    // Get Browser Data Model Count
    int bdmCount = browserDataModel.count();

    // Init result
    int result = 0;

    // Go Thru Items
    for (int i=0; i<bdmCount; i++) {
        // Get Item
        BrowserDataObject* item = static_cast<BrowserDataObject*>(browserDataModel[i]);

        // Check If File Selected
        if (item && item->fileSelected) {
            // Inc Result
            result++;
        }
    }

    return result;
}

//==============================================================================
// Stop Worker
//==============================================================================
void MainBrowserWindow::stopWorkerThread()
{
    qDebug() << "MainBrowserWindow::stopWorkerThread";

    // Terminate
    workerThread.terminate();
    // Wait
    //workerThread.wait();
}

//==============================================================================
// Ensure Current Tree View Item Visible
//==============================================================================
void MainBrowserWindow::ensureCurrentTreeItemVisible()
{
    qDebug() << "MainBrowserWindow::ensureCurrentTreeItemVisible";

    // Expand Current Index
    ui->fileSystemTreeView->expand(ui->fileSystemTreeView->currentIndex());
    // Scroll To Current Index
    ui->fileSystemTreeView->scrollTo(ui->fileSystemTreeView->currentIndex());
}

//==============================================================================
// Get Data Count
//==============================================================================
int MainBrowserWindow::getDataCount()
{
    return browserDataModel.count();
}

//==============================================================================
// Set Thumbs Width
//==============================================================================
void MainBrowserWindow::setThumbsWidth(const int& aThumbsWidth)
{
    // Check Thumbs Width
    if (thumbsWidth != aThumbsWidth) {
        // Set Thumbs Width
        thumbsWidth = qBound(DEFAULT_THUMBS_WIDTH_MIN, aThumbsWidth, DEFAULT_THUMBS_WIDTH_MAX);

        // Emit Thumbs Width Signal Changed
        emit thumbsWidthChanged(thumbsWidth);
    }
}

//==============================================================================
// Get Thumbs Width
//==============================================================================
int MainBrowserWindow::getThumbsWith()
{
    return thumbsWidth;
}

//==============================================================================
// Set Thumbs height
//==============================================================================
void MainBrowserWindow::setThumbsHeight(const int& aThumbsHeight)
{
    // Check Thumbs Height
    if (thumbsHeight != aThumbsHeight) {
        // Set Thumbs Height
        thumbsHeight = qBound(DEFAULT_THUMBS_HEIGHT_MIN, aThumbsHeight, DEFAULT_THUMBS_HEIGHT_MAX);

        // Emit Thumbs Height Signal Changed
        emit thumbsHeightChanged(thumbsHeight);
    }
}

//==============================================================================
// Get Thums Height
//==============================================================================
int MainBrowserWindow::getThumbsHeight()
{
    return thumbsHeight;
}

//==============================================================================
// Set Slide Show Direction
//==============================================================================
void MainBrowserWindow::setSlideShowDirection(const int& aDirection)
{
    // Check Direction
    if (slideShowDirection != aDirection) {
        // Set Direction
        slideShowDirection = qBound(0, aDirection, (int)SSDRandom);

        // Emit Direction Changed Signal
        emit slideShowDirectionChanged(slideShowDirection);
    }
}

//==============================================================================
// Get Slide Show Direction
//==============================================================================
int MainBrowserWindow::getSlideShowDirection()
{
    return slideShowDirection;
}

//==============================================================================
// Set Slide Show Delay
//==============================================================================
void MainBrowserWindow::setSlideShowDelay(const int& aDelay)
{
    // Check Slide Show Delay
    if (slideShowDelay != aDelay) {
        // Set Slide Show Delay
        slideShowDelay = qBound(DEFAULT_SLIDE_SHOW_DELAY_MIN, aDelay, DEFAULT_SLIDE_SHOW_DELAY_MAX);

        // Emit Slide Show Direction Delay Changed Signal
        emit slideShowDelayChanged(slideShowDelay);
    }
}

//==============================================================================
// Get Slide Show Delay
//==============================================================================
int MainBrowserWindow::getSlideShowDelay()
{
    return slideShowDelay;
}

//==============================================================================
// Set Slide Show Wrap Around
//==============================================================================
void MainBrowserWindow::setSlideShowWrap(const bool& aWrapAround)
{
    // Chec kSlide Show Wrap Around
    if (slideShowWrap != aWrapAround) {
        // Set Slide Show Wrap Around
        slideShowWrap = aWrapAround;

        // Emit Slide Show Wrap Around Changed Signal
        emit slideShowWrapChanged(slideShowWrap);
    }
}

//==============================================================================
// Get Slide Show Wrap Around
//==============================================================================
bool MainBrowserWindow::getSlideShowWrap()
{
    return slideShowWrap;
}

//==============================================================================
// File System Model Directory Loaded
//==============================================================================
void MainBrowserWindow::fsModelDirectoryLoaded(const QString& aPath)
{
    Q_UNUSED(aPath);

    //qDebug() << "MainBrowserWindow::fsModelDirectoryLoaded - aPath: " << aPath;

    // ...
}

//==============================================================================
// Tree View Item Activated
//==============================================================================
void MainBrowserWindow::treeItemActivated(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    qDebug() << "MainBrowserWindow::treeItemActivated";

}

//==============================================================================
// Tree View Item Clicked
//==============================================================================
void MainBrowserWindow::treeItemClicked(const QModelIndex& aIndex)
{
    //qDebug() << "MainBrowserWindow::treeItemClicked";

    // Check File System Model
    if (fsModel) {
        // Get Data
        QVariant data = fsModel->data(aIndex);
        qDebug() << "MainBrowserWindow::treeItemClicked - data: " << data.toString();

        // Get File Info
        QFileInfo itemFileInfo = fsModel->fileInfo(aIndex);

        //qDebug() << "MainBrowserWindow::treeItemClicked - fileInfo: " << itemFileInfo.canonicalFilePath();

        // Check If Is Dir
        if (itemFileInfo.isDir() || itemFileInfo.isBundle()) {
            // Set Current Dir
            setCurrentDir(itemFileInfo.canonicalFilePath());
        } else {
            // Set Current File
            setCurrentFile(itemFileInfo.canonicalFilePath());
        }

        // ...
    }
}

//==============================================================================
// Tree View Item DoubleClicked
//==============================================================================
void MainBrowserWindow::treeItemDoubleClicked(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    qDebug() << "MainBrowserWindow::treeItemDoubleClicked";

}

//==============================================================================
// Tree View Item Entered
//==============================================================================
void MainBrowserWindow::treeItemEntered(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    //qDebug() << "MainBrowserWindow::treeItemEntered";

}

//==============================================================================
// Tree View Item Pressed
//==============================================================================
void MainBrowserWindow::treeItemPressed(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    //qDebug() << "MainBrowserWindow::treeItemPressed";

}

//==============================================================================
// Tree View Item Collapsed
//==============================================================================
void MainBrowserWindow::treeItemCollapsed(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    //qDebug() << "MainBrowserWindow::treeItemCollapsed";

}

//==============================================================================
// Tree View Item Expanded
//==============================================================================
void MainBrowserWindow::treeItemExpanded(const QModelIndex& aIndex)
{
    Q_UNUSED(aIndex);

    //qDebug() << "MainBrowserWindow::treeItemExpanded";

}

//==============================================================================
// Tree View Current Item Changed
//==============================================================================
void MainBrowserWindow::currentChanged(const QModelIndex& aCurrent, const QModelIndex& aPrevious)
{
    Q_UNUSED(aCurrent);
    Q_UNUSED(aPrevious);

    //qDebug() << "MainBrowserWindow::currentChanged";

    // ...
}

//==============================================================================
// Browser Widget Mouse Pressed Slot
//==============================================================================
void MainBrowserWindow::browserWidgetMousePressed(const QPoint& aPos, const Qt::MouseButton& aButton)
{
    //qDebug() << "MainBrowserWindow::browserWidgetMousePressed";

    emit browserMousePressed(aPos.x(), aPos.y(), (int)aButton);
}

//==============================================================================
// Browser Widget Mouse Released Slot
//==============================================================================
void MainBrowserWindow::browserWidgetMouseReleased(const QPoint& aPos, const Qt::MouseButton& aButton)
{
    //qDebug() << "MainBrowserWindow::currentChanged";

    emit browserMouseReleased(aPos.x(), aPos.y(), (int)aButton);
}

//==============================================================================
// Worker Result Ready Slot
//==============================================================================
void MainBrowserWindow::workerResultReady(const int& aOperation, const int& aResult)
{
    qDebug() << "MainBrowserWindow::workerResultReady - aOperation: " << aOperation << " - aResult: " << aResult;

    // Quit
    workerThread.quit();

    // Switch Operation
    switch (aOperation) {
        case OTFindDuplicates:
            // Check Result
            if (aResult == 0) {
                // Check Duplicates Search Result Dialog
                if (!dsrDialog) {
                    // Create Result Dialog
                    dsrDialog = new DuplicateSearchResultDialog();
                }

                // Show Duplicates Search Result Dialog
                if (dsrDialog->showDialog(duplicateListModel)) {

                    // Get Duplicate List Model Count
                    int dmCount = duplicateListModel->rowCount();

                    // Go Thru Duplicate Model
                    for (int i=0; i<dmCount; ++i) {
                        // Find Files In Browser Data Model
                        QStringList fileList = duplicateListModel->data(duplicateListModel->index(i), DuplicateListModel::ImageList).toStringList();

                        // Get File List Count
                        int flCount = fileList.count();

                        // Go Thru File List
                        for (int n=0; n<flCount; ++n) {
                            // Get File Name
                            QString fileName = fileList[n];

                            // Check File Name
                            if (fileName.endsWith(DUPLICATE_FILE_LIST_ITEM_SELECTED_SUFFIX)) {
                                // Init File Info
                                QFileInfo fileInfo(fileName.left(fileName.length() - 2));
                                // Set File Selected
                                setFileSelected(findIndex(fileInfo.fileName()), true);
                            }
                        }
                    }

                    // Set Delete Selected Only
                    deleteSelectedOnly = true;

                    // Delete Selected Files
                    deleteFile();
                }
            }

        break;

        default:

        break;
    }

    // Reset Delete Selected Only
    deleteSelectedOnly = false;

    // ...
}

//==============================================================================
// File Renamed Slot
//==============================================================================
void MainBrowserWindow::fileRenamed(const QString& aFileName)
{
    qDebug() << "MainBrowserWindow::fileRenamed - aFileName: " << aFileName;

    // Clear Browser Model
    clearBrowserModel();
    // Update Browser Model
    updateBrowserModel();

    // Get File Index
    int fileIndex = findIndex(aFileName);

    // Populate Browser Model
    populateBrowserModel(fileIndex);

    // ...
}

//==============================================================================
// Custom Context Menu Requested Slot
//==============================================================================
void MainBrowserWindow::fileSystemTreeViewCustomContextMenuRequested(const QPoint& aPoint)
{
    // Get Model Index
    QModelIndex modelIndex = ui->fileSystemTreeView->indexAt(aPoint);

    // Check Model Index
    if (modelIndex.isValid()) {
        //qDebug() << "MainBrowserWindow::fileSystemTreeViewCustomContextMenuRequested - modelIndex.data: " << modelIndex.data().toString();

        // Init Context Menu
        QMenu contextMenu;

        // Set Title
        contextMenu.setTitle(modelIndex.data().toString());

        // Add Action
        contextMenu.addAction(new QAction(FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_CREATE_NEW, &contextMenu));
        contextMenu.addSeparator();
        contextMenu.addAction(new QAction(FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_DELETE, &contextMenu));

        // Exec Context Menu
        QAction* selectedAction = contextMenu.exec(ui->fileSystemTreeView->mapToGlobal(aPoint));

        // Execute Context Menu
        if (selectedAction) {
            qDebug() << "MainBrowserWindow::fileSystemTreeViewCustomContextMenuRequested - selectedAction: " << selectedAction->text();

            // Check Selected Action
            if (selectedAction->text() == QString(FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_CREATE_NEW)) {
                // Init New Dir Dialog
                CreateDirDialog newDirDialog;
                // Execute new Dir Dialog
                if (newDirDialog.exec()) {
                    // Get File System Model
                    QFileSystemModel* fileSystemModel = static_cast<QFileSystemModel*>(ui->fileSystemTreeView->model());
                    // Create New Directory
                    fileSystemModel->mkdir(modelIndex, newDirDialog.getNewDirName());
                }
            // Check Selected Action
            } else if (selectedAction->text() == QString(FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_DELETE)) {
                // Init Confirmation Dialog
                ConfirmationDialog confirmDialog;
                // Set Confirmation Text
                confirmDialog.setConfirmationText(QString("Are you sure to Delete: %1").arg(modelIndex.data().toString()));
                // Launch Confirmation Dialog
                if (confirmDialog.exec()) {
                    // Get File System Model
                    QFileSystemModel* fileSystemModel = static_cast<QFileSystemModel*>(ui->fileSystemTreeView->model());
                    // Remove Directory
                    if (!fileSystemModel->rmdir(modelIndex)) {
                        //qDebug() << "MainBrowserWindow::fileSystemTreeViewCustomContextMenuRequested - ERROR REMOVING DIR!: " << modelIndex.data().toString();
                        // Init Info Dialog
                        InfoDialog infoDialog;
                        // Exec Info Dialog
                        infoDialog.showDialogWithText(QString("Could not remove Directory: %1. Directory might not be empty.").arg(modelIndex.data().toString()));
                    }
                }
            }

            // ...

        }
    }
}

//==============================================================================
// Launch Preferences
//==============================================================================
void MainBrowserWindow::on_actionPreferences_triggered()
{
    // Launch Preferences
    launchPreferences();
}

//==============================================================================
// Action About Triggered
//==============================================================================
void MainBrowserWindow::on_actionAbout_triggered()
{
    // Launch About Dialog
    launchAboutDialog();
}

//==============================================================================
// Action Rotate Left Triggered
//==============================================================================
void MainBrowserWindow::on_actionRotate_Left_triggered()
{
    // Rotate Let
    rotateLeft();
}

//==============================================================================
// Action Rotate Right Triggered
//==============================================================================
void MainBrowserWindow::on_actionRotate_Right_triggered()
{
    // Rotate Right
    rotateRigth();
}

//==============================================================================
// Action Flip Horizontally Triggered
//==============================================================================
void MainBrowserWindow::on_actionFlip_Horizontally_triggered()
{
    // Flip Horizontally
    flipHorizontally();
}

//==============================================================================
// Action Flip Vertically Triggered
//==============================================================================
void MainBrowserWindow::on_actionFlip_Vertically_triggered()
{
    // Flip Vertically
    flipVertically();
}

//==============================================================================
// Action Delete Triggered
//==============================================================================
void MainBrowserWindow::on_actionDelete_triggered()
{
    // Delete File
    deleteFile();
}

//==============================================================================
// Action Copy To Triggered
//==============================================================================
void MainBrowserWindow::on_actionCopy_To_triggered()
{
    // Copy To File(s)
    copyToDirectory();
}

//==============================================================================
// Action Move To Triggered
//==============================================================================
void MainBrowserWindow::on_actionMove_To_triggered()
{
    // Move To Directory
    moveToDirectory();
}

//==============================================================================
// Action Select All Triggered
//==============================================================================
void MainBrowserWindow::on_actionSelect_All_triggered()
{
    // Select All Files
    selectAll();

    // Set Action Enabled
    ui->actionSelect_All->setEnabled(true);
}

//==============================================================================
// Action Select None Triggered
//==============================================================================
void MainBrowserWindow::on_actionSelect_None_triggered()
{
    // Select None
    selectNone();

    // Set Action Enabled
    ui->actionSelect_All->setEnabled(true);
}

//==============================================================================
// Action Rename Triggered
//==============================================================================
void MainBrowserWindow::on_actionRename_triggered()
{
    // Rename Current/Selected File(s)
    renameFile();
}

//==============================================================================
// Action Rename Series Triggered
//==============================================================================
void MainBrowserWindow::on_actionRename_Series_triggered()
{
    // Rename Series/Selected Files
    renameFile();
}

//==============================================================================
// Action Slide Show Triggered
//==============================================================================
void MainBrowserWindow::on_actionSlide_Show_triggered()
{
    // Toggle Slide Show
    toggleSlideShow();
}

//==============================================================================
// Action Compare Images Triggered
//==============================================================================
void MainBrowserWindow::on_actionCompare_Images_triggered()
{
    // Compare Images
    compareImages();
}

//==============================================================================
// Action Refresh Triggered
//==============================================================================
void MainBrowserWindow::on_actionRefresh_triggered()
{
    // Refresh View
    refreshView();
}

//==============================================================================
// Action Find Duplicates Triggered
//==============================================================================
void MainBrowserWindow::on_actionFind_Duplicates_triggered()
{
    // Find Duplicates
    findDuplicates();
}

//==============================================================================
// Action Sort by Name Triggered Slot
//==============================================================================
void MainBrowserWindow::on_actionSort_by_Name_triggered()
{
    // Set Sort Type
    setSortType(DEFAULT_SORT_TYPE_NAME);

    // Update Menu
    updateMenu();
}

//==============================================================================
// Action Sort by Type Triggered Slot
//==============================================================================
void MainBrowserWindow::on_actionSort_by_Type_triggered()
{
    // Set Sort Type
    setSortType(DEFAULT_SORT_TYPE_TYPE);
}

//==============================================================================
// Action Sort by Size Triggered Slot
//==============================================================================
void MainBrowserWindow::on_actionSort_by_Size_triggered()
{
    // Set Sort Type
    setSortType(DEFAULT_SORT_TYPE_SIZE);
}

//==============================================================================
// Action Sort by Date Triggered Slot
//==============================================================================
void MainBrowserWindow::on_actionSort_by_Date_triggered()
{
    // Set Sort Type
    setSortType(DEFAULT_SORT_TYPE_DATE);
}

//==============================================================================
// Action Reverse Sort Order Triggered Slot
//==============================================================================
void MainBrowserWindow::on_actionReverse_triggered()
{
    // Set Sort Reverse Order
    setReverseOrder(ui->actionReverse->isChecked());
}

//==============================================================================
// Action Quit Triggered
//==============================================================================
void MainBrowserWindow::on_actionQuit_triggered()
{
    // Exit Application
    QApplication::exit();
}

//==============================================================================
// Timer Event
//==============================================================================
void MainBrowserWindow::timerEvent(QTimerEvent* aEvent)
{
    // Check Event
    if (aEvent) {
        // Check Timer ID
        if (aEvent->timerId() == slideShowTimerID) {
            qDebug() << "MainBrowserWindow::timerEvent - slideShowTimerID";
            // Get Browser Data Model Count
            int bdmCount = browserDataModel.count();
            // Init Next Index
            int nextIndex = currentIndex;

            // Switch Slide Show Direction
            switch (slideShowDirection) {
                case SSDForward:
                    // Check Next Index
                    if (nextIndex < bdmCount-1) {
                        // Inc Next Index
                        nextIndex++;
                    } else if (slideShowWrap) {
                        // Reset Next Index
                        nextIndex = 0;
                    }
                break;

                case SSDReversed:
                    // Check Next Index
                    if (nextIndex > 0) {
                        // Dec Next Index
                        nextIndex--;
                    } else if (slideShowWrap) {
                        nextIndex = bdmCount - 1;
                    }
                break;

                case SSDRandom:
                    // Set Next Index
                    nextIndex = qrand() % bdmCount;
                break;

                default:
                break;
            }

            // Set Current Index
            setCurrentIndex(nextIndex);
        }
    }
}

//==============================================================================
// Destructor
//==============================================================================
MainBrowserWindow::~MainBrowserWindow()
{
    // Terminate Worker Thread
    stopWorkerThread();

    // Save Settings
    saveSettings();

    // Clear Browser Model
    clearBrowserModel();

    // Delete Browser Widget First To Avoid Crash
    delete ui->browserWidget;

    // Delete Preview Widget First To Avoid Crash
    delete ui->previewWidget;

    // Delete UI
    delete ui;

    // Delete Sorting Group
    delete sortingGroup;

    // Reset File System Model
    fsModel = NULL;

    // Check About Dialog
    if (aboutDialog) {
        // Delete Dialog
        delete aboutDialog;
        aboutDialog = NULL;
    }

    // Check Dir Selector
    if (dirSelector) {
        // Delete Dialog
        delete dirSelector;
        dirSelector = NULL;
    }

    // Check Rename Series Dialog
    if (renameSeriesDialog) {
        // Delete Dialog
        delete renameSeriesDialog;
        renameSeriesDialog = NULL;
    }

    // Check Info Dialog
    if (infoDialog) {
        // Delete Dialog
        delete infoDialog;
        infoDialog = NULL;
    }

    // Check Comapre Dialog
    if (compareDialog) {
        // Delete Dialog
        delete compareDialog;
        compareDialog = NULL;
    }

    // Check DSR Dialog
    if (dsrDialog) {
        // Delete Dialog
        delete dsrDialog;
        dsrDialog = NULL;
    }

    // Check Duplicate List Model
    if (duplicateListModel) {
        delete duplicateListModel;
        duplicateListModel = NULL;
    }

    qDebug() << "MainBrowserWindow::~MainBrowserWindow";
}

