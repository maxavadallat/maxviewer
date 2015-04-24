#ifndef MAINBROWSERWINDOW_H
#define MAINBROWSERWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QString>
#include <QSettings>
#include <QThread>
#include <QTimerEvent>
#include <QMultiMap>
#include <QActionGroup>

#include "constants.h"

namespace Ui {
class MainBrowserWindow;
}

class MainViewerWindow;
class BrowserDataObject;
class AboutForm;
class DirSelectorDialog;
class RenameSeriesDialog;
class RenameFileDialog;
class InfoDialog;
class CompareDialog;
class DuplicateSearchResultDialog;
class ImageCompareFrame;
class Worker;
class DuplicateListModel;

//==============================================================================
// Main Browser Window
//==============================================================================
class MainBrowserWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QString currentDir READ getCurrentDir WRITE setCurrentDir NOTIFY currentDirChanged)
    Q_PROPERTY(QString currentFile READ getCurrentFile WRITE setCurrentFile NOTIFY currentFileChanged)
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int popupIndex READ getPopupIndex WRITE setPopupIndex NOTIFY popupIndexChanged)
    Q_PROPERTY(int dataCount READ getDataCount NOTIFY dataCountChanged)

    Q_PROPERTY(int thumbsWidth READ getThumbsWith WRITE setThumbsWidth NOTIFY thumbsWidthChanged)
    Q_PROPERTY(int thumbsHeight READ getThumbsHeight WRITE setThumbsHeight NOTIFY thumbsHeightChanged)
    Q_PROPERTY(int slideShowDirection READ getSlideShowDirection WRITE setSlideShowDirection NOTIFY slideShowDirectionChanged)
    Q_PROPERTY(int slideShowDelay READ getSlideShowDelay WRITE setSlideShowDelay NOTIFY slideShowDelayChanged)
    Q_PROPERTY(bool slideShowWrap READ getSlideShowWrap WRITE setSlideShowWrap NOTIFY slideShowWrapChanged)

public:

    // Static Constructor
    static MainBrowserWindow* getInstance();
    // Release Instance
    void release();

    // Set Current Dir
    void setCurrentDir(const QString& aCurrentDir);
    // Get Current Dir
    QString getCurrentDir();

    // Set Current File
    void setCurrentFile(const QString& aCurrentFile);
    // Get Current File
    QString getCurrentFile();

    // Set Current Index
    void setCurrentIndex(const int& aCurrentIndex);
    // Get Current Index
    int getCurrentIndex();

    // Set Popup Index
    void setPopupIndex(const int& aPopupIndex);
    // Get Popup Index
    int getPopupIndex();


    // Ensure Current Tree View Item Visible
    void ensureCurrentTreeItemVisible();

    // Get Count
    int getDataCount();

    // Set Thumbs Width
    void setThumbsWidth(const int& aThumbsWidth);
    // Get Thumbs Width
    int getThumbsWith();

    // Set Thumbs height
    void setThumbsHeight(const int& aThumbsHeight);
    // Get Thums Height
    int getThumbsHeight();

    // Set Slide Show Direction
    void setSlideShowDirection(const int& aDirection);
    // Get Slide Show Direction
    int getSlideShowDirection();

    // Set Slide Show Delay
    void setSlideShowDelay(const int& aDelay);
    // Get Slide Show Delay
    int getSlideShowDelay();

    // Set Slide Show Wrap Around
    void setSlideShowWrap(const bool& aWrapAround);
    // Get Slide Show Wrap Around
    bool getSlideShowWrap();

protected:

    // Constructor
    explicit MainBrowserWindow(QWidget* aParent = NULL);

    // Destructor
    virtual ~MainBrowserWindow();

public slots:

    // Launch Viewer
    void launchViewer(const QString& aFilePath = QString(""));
    // Hide Viewer
    void hideViewer();

    // Toggle Viewer Fullscreen
    void toggleViewerFullScreen();

    // Launch Preferences
    void launchPreferences();

    // Launch About Dialog
    void launchAboutDialog();

    // Toggle Slide Show
    void toggleSlideShow();

    // Show Window
    void showWindow();
    // Hide Window
    void hideWindow();

    // Next Image
    void nextImage();
    // Prev Image
    void prevImage();

    // Change Dir Up
    void cdUp();

    // Get File Size In String Format
    QString getFileSize(const QString& aFilePath);

    // Show Status Text Slot
    void showStatusText(const QString& aStatusText, const int& aTimeout = 0);

    // Handle File Open Event Slot
    void handleFileOpenEvent(const QString& aFileName);

    // Rotate Current/Selected Image(s) Left
    void rotateLeft();
    // Rotate Current/Selected Image(s) Right
    void rotateRigth();
    // Flip Current/Selected Image(s) Horizontally
    void flipHorizontally();
    // Flip Current/Selected Image(s) Vertically
    void flipVertically();
    // Delete Current/Selected File(s)
    void deleteFile();
    // Rename Current/Selected File
    void renameFile();
    // Copy Current/Selected File(s)
    void copyToDirectory();
    // Move Current/Selected File(s)
    void moveToDirectory();

    // Select All Files in Current Dir
    void selectAll();
    // Select None Files in Current Dir
    void selectNone();

    // Set File Selected By Index
    void setFileSelected(const int& aIndex, const bool& aSelected);

    // Has Selection
    bool hasSelection();

    // Get Number Of Selected Items
    int numberOfSelected();

    // Stop Worker
    void stopWorkerThread();

    // Start Slide Show
    void startSlideShow();
    // Stop Slide Show
    void stopSlideShow();

    // Register App For Supported Images
    void registerAppForImages();

    // Compare Images
    void compareImages();

    // Find Duplicates in the Current Directory
    void findDuplicates();

    // Find Model Index By File Name
    int findIndex(const QString& aFileName);

    // Refresh View
    void refreshView(const int& aIndex = -1);

    // Set Sorting Type
    void setSortType(const int& aSortType);

    // Set Sort Reverse Order
    void setReverseOrder(const bool& aReverse);

signals:

    // Current Dir Changed Signal
    void currentDirChanged(const QString& aCurrentDir);
    // Current File Changed Signal
    void currentFileChanged(const QString& aCurrentFile);
    // Current Index Changed Signal
    void currentIndexChanged(const int& aCurrentIndex);
    // Popup Index Changed Signal
    void popupIndexChanged(const int& aPopupIndex);
    // Data Count Changed Signal
    void dataCountChanged(const int& aCount);

    // Thumbs Width Changed Signal
    void thumbsWidthChanged(const int& aWidth);
    // Thumbs Height Changed Signal
    void thumbsHeightChanged(const int& aHeight);
    // Slide Show Direction Changed Signal
    void slideShowDirectionChanged(const int& aDirection);
    // Slide Show Delay Changed Signal
    void slideShowDelayChanged(const int& aDelay);
    // Slide Show Wrap Around Changed Signal
    void slideShowWrapChanged(const bool& aWrapAround);
    // Current File Updated Signal
    void currentFileUdated();

    // File Updated Signal
    void fileUpdated(const int& aIndex);

    // Deleting Index Signal
    void deletingIndex(const int& aIndex);
    // Index Deleted Signal
    void indexDeleted(const int& aIndex);

    // Browser Widget Focus Changed Signal
    void browserFocusChanged(const bool& aFocused);

    // Browser Widget Mouse Pressed Signal
    void browserMousePressed(const int& aPosX, const int& aPosY, const int& aButton);
    // Browser Widget Mouse Released Signal
    void browserMouseReleased(const int& aPosX, const int& aPosY, const int& aButton);

    // Progress Changed Signal
    void progressChanged(const int& aTotal, const int& aProgress);

    // Operate Worker Signal
    void operateWorker(const int& aOperation);

private slots:

    // File System Model Directory Loaded
    void fsModelDirectoryLoaded(const QString& aPath);

    // Tree View Item Activated
    void treeItemActivated(const QModelIndex& aIndex);
    // Tree View Item Clicked
    void treeItemClicked(const QModelIndex& aIndex);
    // Tree View Item DoubleClicked
    void treeItemDoubleClicked(const QModelIndex& aIndex);
    // Tree View Item Entered
    void treeItemEntered(const QModelIndex& aIndex);
    // Tree View Item Pressed
    void treeItemPressed(const QModelIndex& aIndex);

    // Tree View Item Collapsed
    void treeItemCollapsed(const QModelIndex& aIndex);
    // Tree View Item Expanded
    void treeItemExpanded(const QModelIndex& aIndex);

    // Tree View Current Item Changed
    void currentChanged(const QModelIndex& aCurrent, const QModelIndex& aPrevious);

    // Browser Widget Mouse Pressed Slot
    void browserWidgetMousePressed(const QPoint& aPos, const Qt::MouseButton& aButton);
    // Browser Widget Mouse Released Slot
    void browserWidgetMouseReleased(const QPoint& aPos, const Qt::MouseButton& aButton);

    // Populate Browser Model
    void populateBrowserModel(const int& aIndex);

    // Worker Result Ready Slot
    void workerResultReady(const int& aOperation, const int& aResult);

    // File Renamed Slot
    void fileRenamed(const QString& aFileName);

    // Custom Context Menu Requested Slot
    void fileSystemTreeViewCustomContextMenuRequested(const QPoint& aPoint);

    // Action Preferences Triggered Slot
    void on_actionPreferences_triggered();
    // Action About Triggered Slot
    void on_actionAbout_triggered();
    // Action Rotate Left Triggered Slot
    void on_actionRotate_Left_triggered();
    // Action Rotate Right Triggered Slot
    void on_actionRotate_Right_triggered();
    // Action Flip Horizontally Triggered Slot
    void on_actionFlip_Horizontally_triggered();
    // Action Flip Vertically Triggered Slot
    void on_actionFlip_Vertically_triggered();
    // Action Delete Triggered Slot
    void on_actionDelete_triggered();
    // Action Copy To Triggered Slot
    void on_actionCopy_To_triggered();
    // Action Move To Triggered Slot
    void on_actionMove_To_triggered();
    // Action Select All Triggered Slot
    void on_actionSelect_All_triggered();
    // Action Select None Triggered Slot
    void on_actionSelect_None_triggered();
    // Action Rename Triggered Slot
    void on_actionRename_triggered();
    // Action REname Series Triggered Slot
    void on_actionRename_Series_triggered();
    // Action Slide Show Triggered Slot
    void on_actionSlide_Show_triggered();
    // Action Compare Images Triggered Slot
    void on_actionCompare_Images_triggered();
    // Action Refresh Triggered Slot
    void on_actionRefresh_triggered();
    // Action Find Duplicates Slot
    void on_actionFind_Duplicates_triggered();
    // Action Sort by Name Triggered Slot
    void on_actionSort_by_Name_triggered();
    // Action Sort by Type Triggered Slot
    void on_actionSort_by_Type_triggered();
    // Action Sort by Size Triggered Slot
    void on_actionSort_by_Size_triggered();
    // Action Sort by Date Triggered Slot
    void on_actionSort_by_Date_triggered();
    // Action Quit Triggered Slot
    void on_actionQuit_triggered();

    void on_actionReverse_triggered();

protected:

    // Timer Event
    virtual void timerEvent(QTimerEvent* aEvent);

private:
    friend class Worker;

    // Setup Tree View
    void setupTreeView();
    // Setup Browser Grid
    void setupBrowserGrid();
    // Setup Preview
    void setupPreview();

    // Restore UI
    void restoreUI();
    // Update Menu
    void updateMenu();

    // Init Worker
    void initWorker();

    // Save Settings
    void saveSettings();

    // Update Browser Model
    void updateBrowserModel();
    // Clear Browser Model
    void clearBrowserModel();

    // Rotate Current/Selected Image(s) Left
    void doRotateLeft();
    // Rotate Current/Selected Image(s) Right
    void doRotateRigth();
    // Flip Current/Selected Image(s) Horizontally
    void doFlipHorizontally();
    // Flip Current/Selected Image(s) Vertically
    void doFlipVertically();
    // Delete Current/Selected File(s)
    void doDeleteFile();
    // Rename Current/Selected File(s)
    void doRenameFile();
    // Copy Current/Selected File(s)
    void doCopyToDirectory();
    // Move Current/Selected File(s)
    void doMoveToDirectory();

    // Find Duplicate Files
    void doFindDuplicates();

    // Delete File By Index
    void deleteFileByIndex(const int& aIndex, const bool& aDeleteItem = true, const bool& aNotify = true);

    // Rotate File By Index
    void rotateFileByIndex(const int& aIndex, int aDirection, const bool& aNotify = true);
    // Flip File By Index
    void flipFileByIndex(const int& aIndex, int aDirection = FDTHorizontal, const bool& aNotify = true);

    // Copy File By Index
    void copyFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aNotify = true);
    // Move File By Index
    void moveFileByIndex(const int& aIndex, const QString& aTargetDir, const bool& aDeleteItem = true, const bool& aNotify = true);

    // Rename File By Index
    void renameFileByIndex(const int& aIndex, const QString& aFileName, const bool& aNotify = true);

    // Evaluate File Rename Pattern
    bool evaluateFileRenamePattern();

private:

    // UI
    Ui::MainBrowserWindow*          ui;
    // Sorting Action Group
    QActionGroup*                   sortingGroup;
    // File System Model
    QFileSystemModel*               fsModel;

    // Current Dir
    QString                         currentDir;
    // Current File
    QString                         currentFile;
    // Current Index
    int                             currentIndex;
    // Popup Index
    int                             popupIndex;

    // Browser Data Model
    QList<QObject*>                 browserDataModel;
    // Deleted Browser Data Model Items
    QList<QObject*>                 deletedItems;

    // Viewer Window
    MainViewerWindow*               viewerWindow;
    // About Form
    AboutForm*                      aboutDialog;
    // Dir Selector Dialog
    DirSelectorDialog*              dirSelector;
    // Rename Seies Dialog
    RenameSeriesDialog*             renameSeriesDialog;
    // Rename File Dialog
    RenameFileDialog*               renameFileDialog;
    // Info Dialog
    InfoDialog*                     infoDialog;
    // Compare Dialog
    CompareDialog*                  compareDialog;
    // Duplicate Search Result Dialog
    DuplicateSearchResultDialog*    dsrDialog;

    // Thumbs Width
    int                             thumbsWidth;
    // Thumbs Height
    int                             thumbsHeight;
    // Slide Show Direction
    int                             slideShowDirection;
    // Slide Show Delay
    int                             slideShowDelay;
    // Slide Show Wrap Aroound
    bool                            slideShowWrap;

    // Sort Type
    int                             sortType;
    // Reverse Order
    bool                            reverseOrder;

    // Selected Transfer Dir
    QString                         transferDir;

    // File Operation Worker
    Worker*                         worker;

    // Worker Thread
    QThread                         workerThread;

    // Slide Show timer ID
    int                             slideShowTimerID;

    // Duplicate List Model
    DuplicateListModel*             duplicateListModel;

    // Delete Selected Files Only, No Current File Deletion
    bool                            deleteSelectedOnly;
};

#endif // MAINBROWSERWINDOW_H
