#ifndef CONSTANTS_H
#define CONSTANTS_H

// Application Info

#define DEFAULT_APPLICATION_NAME                        "MaxViewer"
#define DEFAULT_ORGANIZATION_NAME                       "MySoft"
#define DEFAULT_ORGANIZATION_DOMAIN                     "mysoft.com"


// QML Context Properties

#define MAIN_VIEW_CONTROLLER                            "mainViewController"
#define VIEWER_VIEW_CONTROLLER                          "viewerViewController"
#define MAIN_VIEW_DATA_MODEL                            "mainViewDataModel"

#define DUPLICATE_LIST_MODEL                            "duplicateListModel"


// Settings Groups

#define SETTINGS_GROUP_MAIN                             "main"
#define SETTINGS_GROUP_UI                               "ui"


// Settings Keys

#define SETTINGS_KEY_LAST_DIR                           SETTINGS_GROUP_MAIN"/lastDir"
#define SETTINGS_KEY_LAST_FILE                          SETTINGS_GROUP_MAIN"/lastFile"
#define SETTINGS_KEY_LAST_INDEX                         SETTINGS_GROUP_MAIN"/lastIndex"

#define SETTINGS_KEY_MAIN_SPLITTER                      SETTINGS_GROUP_UI"/mainSplitter"
#define SETTINGS_KEY_LEFT_SPLITTER                      SETTINGS_GROUP_UI"/leftSplitter"

#define SETTINGS_KEY_BROWSER_MAXIMIZED                  SETTINGS_GROUP_UI"/browserMaximized"

#define SETTINGS_KEY_VIEWER_FULLSCREEN                  SETTINGS_GROUP_UI"/viewerFullScreen"

#define SETTINGS_KEY_VIEWER_POSX                        SETTINGS_GROUP_UI"/viewerPosX"
#define SETTINGS_KEY_VIEWER_POSY                        SETTINGS_GROUP_UI"/viewerPosY"
#define SETTINGS_KEY_VIEWER_WIDTH                       SETTINGS_GROUP_UI"/viewerWidth"
#define SETTINGS_KEY_VIEWER_HEIGHT                      SETTINGS_GROUP_UI"/viewerHeight"

#define SETTINGS_KEY_THUMBS_WIDTH                       SETTINGS_GROUP_UI"/thumbsWidth"
#define SETTINGS_KEY_THUMBS_HEIGHT                      SETTINGS_GROUP_UI"/thumbsHeiight"

#define SETTINGS_KEY_SLIDE_SHOW_DIRECTION               SETTINGS_GROUP_UI"/slideShowDirection"
#define SETTINGS_KEY_SLIDE_SHOW_DELAY                   SETTINGS_GROUP_UI"/slideShowDelay"

#define SETTINGS_KEY_SLIDE_SHOW_WRAPAROUND              SETTINGS_GROUP_UI"/slideShowWrap"

#define SETTINGS_KEY_SLIDE_SORT_TYPE                    SETTINGS_GROUP_UI"/sortType"
#define SETTINGS_KEY_SLIDE_SORT_ORDER                   SETTINGS_GROUP_UI"/sortOrder"


// Supported Formats

#define DEFAULT_FILTER_TEMPLATE                         "*.%1"

#define DEFAULT_SUPPORTED_FORMAT_BMP                    "bmp"
#define DEFAULT_SUPPORTED_FORMAT_JPG                    "jpg"
#define DEFAULT_SUPPORTED_FORMAT_JPEG                   "jpeg"
#define DEFAULT_SUPPORTED_FORMAT_GIF                    "gif"
#define DEFAULT_SUPPORTED_FORMAT_PNG                    "png"



// Default Settings Values

#define DEFAULT_THUMBS_WIDTH                            320

#define DEFAULT_THUMBS_WIDTH_MIN                        100
#define DEFAULT_THUMBS_WIDTH_MAX                        400


#define DEFAULT_THUMBS_HEIGHT                           200

#define DEFAULT_THUMBS_HEIGHT_MIN                       100
#define DEFAULT_THUMBS_HEIGHT_MAX                       300

#define DEFAULT_SLIDE_SHOW_DELAY                        3000

#define DEFAULT_SLIDE_SHOW_DELAY_MIN                    1000
#define DEFAULT_SLIDE_SHOW_DELAY_MAX                    10000


#define DEFAULT_SORT_TYPE_NAME                          QDir::Name
#define DEFAULT_SORT_TYPE_TYPE                          QDir::Type
#define DEFAULT_SORT_TYPE_SIZE                          QDir::Size
#define DEFAULT_SORT_TYPE_DATE                          QDir::Time


enum SlideShowDirectionType
{
    SSDForward      = 0,
    SSDReversed,
    SSDRandom
};

enum FlipDirectionType
{
    FDTHorizontal   = 0,
    FDTVertical
};

enum RotationDirectionType
{
    RDTLeft         = 0,
    RDTRight
};

#define DEFAULT_SLIDE_SHOW_DIRECTION                    0

#define DEFAULT_STATUS_TEXT_SHOW_TIMEOUT                3000


#define TRANSFORM_ROTATE_LEFT                           -90
#define TRANSFORM_ROTATE_RIGHT                          90


#define DEFAULT_TRANSFER_LIST_FILE_NAME                 ".transfers.list"



#define FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_CREATE_NEW    "Create New Dir"
#define FILE_SYSTEM_TREEVIEW_CONTEXT_MENU_DELETE        "Delete"


#define DUPLICATE_FILE_LIST_ITEM_SELECTED_SUFFIX        " *"


#define DEFAULT_TRANSFER_OPTIONS_YES_TO_ALL             0x0001
#define DEFAULT_TRANSFER_OPTIONS_NO_TO_ALL              0x0010


#endif // CONSTANTS_H

