import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const
import "qrc:/qml"

Rectangle {
    id: browserGridRoot

    property bool hasFocus: false

    property string highlightColor: hasFocus ? Const.defaultHighlightColorFocused : Const.defaultHighlightColor

    property bool selectionMode: false

    property bool singleSelectionMode: false

    property int lastSelectionIndex: 0

    property int popupPosX: 0

    property int popupPosY: 0

    property bool mouseOverPopup: false

    property bool popupShown: false

    property int hoverIndex: 0

    property bool delegateHasPointer: false

    width: 400
    height: 300

    color: Const.defaultBackgroundColor;

    focus: true

    // Browser Grid View
    GridView {
        id: browserGrid

        property int scrollBarWidth: Const.defaultScrollBarWidth
        property int scrollBarHeight: browserGrid.height * browserGrid.height / browserGrid.contentHeight
        property int scrollBarPosY: browserGrid.contentY * browserGrid.height / browserGrid.contentHeight

        //property int name: value

        anchors.fill: parent
        anchors.margins: 0

        highlightFollowsCurrentItem: true
        highlightMoveDuration: Const.defaultHighlightSpeed

        cellWidth: mainViewController.thumbsWidth + Const.defaultThumbSpacing
        cellHeight: mainViewController.thumbsHeight + Const.defaultThumbSpacing

        snapMode: GridView.SnapPosition

        //cacheBuffer: 2

        // Model
        model: mainViewDataModel

        // Vertical Scrollbar
        Rectangle {
            id: verticalScrollBar
            width: browserGrid.scrollBarWidth
            height: browserGrid.scrollBarHeight
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: browserGrid.scrollBarPosY
            //color: "yellow"
            color: "transparent"
            border.width: 1
            border.color: Const.defaultFontColor
        }

        // Delegate
        delegate: Rectangle {
            width: mainViewController.thumbsWidth
            height: mainViewController.thumbsHeight

            color: "transparent"
            //color: "red"

            radius: 2
            border.width: 1
            border.color: Const.defaultBorderColor

            // Delegate Thumbnail
            Image {
                id: thumbImage
                anchors.fill: parent
                anchors.margins: Const.defaultThumbMargins
                sourceSize.width: mainViewController.thumbsWidth
                sourceSize.height: mainViewController.thumbsHeight
                fillMode: Image.PreserveAspectFit
                cache: false
                smooth: false
                asynchronous: true
                visible: opacity > 0.0
                opacity: thumbImage.status === Image.Ready ? 1.0 : 0.0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 200
                    }
                }

                source: "file://" + mainViewController.currentDir + "/" + fileName

                // On Status Changed
                onStatusChanged: {
                    // Check Index
                    //if (index === mainViewController.currentIndex)
                    {
                        //console.log("thumbImage.onStatusChanged - status: " + thumbImage.status + " - source: " + thumbImage.source);

                        // Check Status - No Image Loaded
                        if (thumbImage.status === Image.Null) {
                            // Set Source
                            thumbImage.source = "file://" + mainViewController.currentDir + "/" + fileName;
                        }

                        // ...
                    }
                }
            }

            // Delegate Selected Icon
            Image {
                id: selectedIcon
                width: Const.defaultSelectedIconSize
                height: Const.defaultSelectedIconSize
                anchors.top: parent.top
                anchors.right: parent.right
                opacity: fileSelected ? 1.0 : 0.0
                visible: opacity > 0.0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 100
                    }
                }
                source: "resources/images/checkbox_checked.png"
            }

            // Loading Text
            Text {
                id: loadingText
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Const.defaultFontSize
                color: Const.defaultFontColor
                visible: opacity > 0.0
                opacity: thumbImage.status !== Image.Ready ? 1.0 : 0.0
                Behavior on opacity {
                    NumberAnimation {
                        duration: 200
                    }
                }

                text: thumbImage.status === Image.Error ? "Error." : "Loading..."
            }

            // Delegate Title Shadow
            Text {
                id: thumbTitleShadow
                width: thumbTitle.width
                //clip: true
                elide: thumbTitle.elide
                font.pixelSize: thumbTitle.font.pixelSize
                anchors.bottom: thumbTitle.bottom
                anchors.bottomMargin: - 1
                anchors.horizontalCenter: thumbTitle.horizontalCenter
                anchors.horizontalCenterOffset: 1
                horizontalAlignment: thumbTitle.horizontalAlignment
                font.bold: thumbTitle.font.bold
                color: Const.defaultFontShadowColor

                text: thumbTitle.text
            }

            // Delegate Title
            Text {
                id: thumbTitle
                width: parent.width
                //clip: true
                elide: Text.ElideRight
                font.pixelSize: Const.defaultFontSize
                anchors.bottom: parent.bottom
                anchors.bottomMargin: Const.defaultThumbMargins * 2
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                color: Const.defaultFontColor

                text: fileName
            }

            // Delegate Mouse Area
            MouseArea {
                id: delegateMouseArea
                anchors.fill: parent
                hoverEnabled: true
                acceptedButtons: Qt.LeftButton /*| Qt.RightButton*/
                // On Clicked
                onClicked: {
                    // Check Modifiers - SHIFT
                    if (mouse.modifiers & Qt.ShiftModifier) {
                        // Set Selection Mode
                        browserGridRoot.selectionMode = true;
                    }

                    // Check Modifiers - META
                    if (mouse.modifiers & Qt.ControlModifier) {
                        // Set Single Selection Mode
                        browserGridRoot.singleSelectionMode = true;
                    }

                    // Check Single Selection Mode
                    if (browserGridRoot.singleSelectionMode) {
                        // Toggle File Selected
                        fileSelected = !fileSelected;
                    } else {
                        // Check Selection Mode
                        if (browserGridRoot.selectionMode) {
                            // Toggle Item Selections
                            //browserGridRoot.toggleItemSelections(mainViewController.currentIndex, index);
                            // Set Item Selections
                            browserGridRoot.setItemSelections(mainViewController.currentIndex, index);

                        } else {
                            // Set Current Index
                            browserGrid.currentIndex = index;
                        }
                    }
                }

                // On Contains Mouse Changed
                onContainsMouseChanged: {
                    //console.log("delegateMouseArea.onContainsMouseChanged - containsMouse: " + delegateMouseArea.containsMouse);

                    // Check Contains Mouse
                    if (delegateMouseArea.containsMouse && !browserGridRoot.popupShown) {
                        // Set Hover Index
                        browserGridRoot.hoverIndex = index;
                    }

                    // Set Delegate Has Pointer
                    browserGridRoot.delegateHasPointer = delegateMouseArea.containsMouse;
                }

                // On Double Clicked
                onDoubleClicked: {
                    // Check Selection Mode
                    if (!browserGridRoot.selectionMode && !browserGridRoot.singleSelectionMode && mouse.button === Qt.LeftButton) {
                        // Set Current Index
                        browserGrid.currentIndex = index;
                        // Enable/Disable Grid
                        browserGrid.interactive = false;
                        browserGrid.interactive = true;

                        // Launch Viewer
                        mainViewController.launchViewer();
                    }
                }

                // On Entered
                onEntered: {
                    //console.log("delegateMouseArea.onEntered - index: " + index);
                }

                // On Exited
                onExited: {
                    //console.log("delegateMouseArea.onExited - index: " + index);
                    // ...
                }

                // On Pressed
                onPressed: {
                    //console.log("delegateMouseArea.onPressed - index: " + index);

                    // Hide Popup Menu
                    hidePopupMenu();

                    // ...
                }

                // On released
                onReleased: {
                    //console.log("delegateMouseArea.onReleased - index: " + index + " - pos[" + mouse.x + ":" + mouse.y + "]");

                    // Chekc Button
                    if (mouse.button === Qt.RightButton) {
                        // Set Popup Index
                        mainViewController.popupIndex = index;
                        // Show Popup Menu
                        showPopupMenu();
                    }

                    // ...
                }
            }

            // Connections
            Connections {
                target: mainViewController
                // On Current File Updated
                onCurrentFileUdated: {
                    // Check Index
                    if (index === mainViewController.currentIndex) {
                        //console.log("browserGrid.delegate.onCurrentFileUpdated - index: " + index);
                        // Reset Thumbnail Source
                        thumbImage.source = "";
                    }
                }

                // On File Updated
                onFileUpdated: {
                    // Check Index
                    if (index === aIndex) {
                        console.log("browserGrid.delegate.onFileUpdated - aIndex: " + aIndex);
                        // Reset Thumbnail Source
                        thumbImage.source = "";

                        // ...
                    }
                }
            }
        }

        // Highlight
        highlight: Rectangle {
            //color: Const.defaultHighlightColorFocused
            color: browserGridRoot.highlightColor
        }

        // On Current Index Chnged
        onCurrentIndexChanged: {
            //console.log("browserGrid.onCurrentIndexChanged - fileName: " + mainViewDataModel[browserGrid.currentIndex].fileName);
            // Set Current Index
            mainViewController.currentIndex = browserGrid.currentIndex;
        }

        // On Width Changed
        onWidthChanged: {
            // Position View At Index
            browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.End);
        }

        // On Height Changed
        onHeightChanged: {
            //console.log("browserGrid.onHeightChanged - height: " + browserGrid.height);

            // Position View At Index
            browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.End);
        }

        onContentXChanged: {
            //console.log("browserGrid.onContentXChanged - constentX: " + browserGrid.contentX);

            // ...
        }

        onContentYChanged: {
            //console.log("browserGrid.onContentYChanged - constentY: " + browserGrid.contentY);

            // ...
        }
    }

    // Progress Indicator
    ProgressIndicator {
        id: progressIndicator

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        anchors.horizontalCenter: parent.horizontalCenter

        opacity: 0.0
    }

    // Popup Menu
    Rectangle {
        id: popupMenu
        width: popupColumn.width
        height: popupColumn.height + radius * 2
        anchors.left: parent.left
        anchors.leftMargin: browserGridRoot.popupPosX
        anchors.top: parent.top
        anchors.topMargin: browserGridRoot.popupPosY

        color: Const.defaultPopupBackgroundColor

        border.color: Const.defaultPopupBorderColor
        border.width: 2

        radius: 8

        visible: opacity > 0.0
        opacity: 0.0

        clip: true

        Behavior on opacity {
            NumberAnimation {
                duration: 100
            }
        }

        // Popup Menu Mouse Area
        MouseArea {
            id: popupMenuMouseArea
            anchors.fill: parent
            hoverEnabled: true
            preventStealing: true
            onEntered: {
                //console.log("popupMenuMouseArea.onEntered");

                // Set Mouse Over Popup
                mouseOverPopup = true;

                // ...
            }
            onExited: {
                //console.log("popupMenuMouseArea.onExited");

                // Reset Mouse Over Popup
                mouseOverPopup = false;

                // ...
            }
        }

        // Popup List View
        Column {
            id: popupColumn

            width: Const.defaultPopupWidth

            anchors.top: parent.top
            anchors.topMargin: 8

            clip: true

            //anchors.fill: parent

            PopupItem {
                title: "Rotate Left"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);
                    // Hide Popup Menu
                    hidePopupMenu();
                    // Rotate Left
                    mainViewController.rotateLeft();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "Rotate Right"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);
                    // Hide Popup Menu
                    hidePopupMenu();
                    // Rotate Right
                    mainViewController.rotateRigth();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "-"
            }

            PopupItem {
                title: "Flip Horizontally"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Flip Horizontally
                    mainViewController.flipHorizontally();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "Flip Vertically"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Flip Vertically
                    mainViewController.flipVertically();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "-"
            }

            PopupItem {
                title: "Copy To..."
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Copy To Directory
                    mainViewController.copyToDirectory();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "Move To..."
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Move To Directory
                    mainViewController.moveToDirectory();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "-"
            }

            PopupItem {
                title: "Rename"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Rename File
                    mainViewController.renameFile();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "-"
            }

            PopupItem {
                title: "Compare..."
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Compare Images
                    mainViewController.compareImages();
                }

                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }

            PopupItem {
                title: "-"
            }

            PopupItem {
                title: "Delete"
                onItemClicked: {
                    //console.log("popupMenu.onItemClicked - " + title);

                    // Hide Popup Menu
                    hidePopupMenu();
                    // Delete File
                    mainViewController.deleteFile();
                }
                onMouseHoveredChanged: {
                    // Set Mouse Over Popup
                    browserGridRoot.mouseOverPopup = mouseHovered;
                }
            }
        }
    }

    // On Completed
    Component.onCompleted: {
        //console.log("browserGridRoot.onCompleted");

        // Reset Content Position
        browserGrid.contentX = 0;
        browserGrid.contentY = 0;

        // ...
    }

    // On Destruction
    Component.onDestruction: {
        //console.log("browserGridRoot.onDestruction");

        // ...
    }

    // On Hover Index Changed
    onHoverIndexChanged: {
        console.log("browserGridRoot.onHoverIndexChanged - hoverIndex: " + hoverIndex);

        // ...
    }

    // Toggle Item Selections
    function toggleItemSelections(aStartIndex, aEndIndex) {
        //console.log("browserGridRoot.toggleItemSelections - start: " + aStartIndex + " - end: " + aEndIndex);
        // Get Item Count
        var mvdmCount = mainViewDataModel.length;
        // Set Start
        var start = aEndIndex > aStartIndex ? aStartIndex : aEndIndex;
        // Set End
        var end = aEndIndex > aStartIndex ? aEndIndex : aStartIndex;

        // Go Thru Items
        for (var i=start; i<=end && i<mvdmCount; i++) {
            // Toggle File Selected
            mainViewDataModel[i].fileSelected = !mainViewDataModel[i].fileSelected;
        }

        // ...
    }

    // Set Item Selections
    function setItemSelections(aStartIndex, aEndIndex) {
        //console.log("browserGridRoot.toggleItemSelections - start: " + aStartIndex + " - end: " + aEndIndex);
        // Get Item Count
        var mvdmCount = mainViewDataModel.length;
        // Set Start
        var start = aEndIndex > aStartIndex ? aStartIndex : aEndIndex;
        // Set End
        var end = aEndIndex > aStartIndex ? aEndIndex : aStartIndex;

        // Go Thru Items
        for (var i=start; i<=end && i<mvdmCount; i++) {
            // Toggle File Selected
            mainViewDataModel[i].fileSelected = true;
        }

        // ...
    }

    // Show Popup
    function showPopupMenu() {
        // Check Opacity
        if (popupMenu.opacity !== 1.0) {
            //console.log("browserGridRoot.showPopupMenu - popupIndex: " + mainViewController.popupIndex);
            // Set Opacity
            popupMenu.opacity = 1.0;

            // Set Popup Shown
            browserGridRoot.popupShown = true;
        }
    }

    // Hide Popup
    function hidePopupMenu() {
        // Check Opacity
        if (popupMenu.opacity !== 0.0) {
            //console.log("browserGridRoot.hidePopupMenu");
            // Reset Opacity
            popupMenu.opacity = 0.0;

            // Set Popup Shown
            browserGridRoot.popupShown = false;
        }

        // Reset Popup Index
        //mainViewController.popupIndex = -1;
    }

    // On Up Pressed
    Keys.onUpPressed: {
        //console.log("browserGridRoot.Keys.onUpPressed");

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Last Selection Index
            lastSelectionIndex = mainViewController.currentIndex;
        }

        // Move Current Index To Up
        browserGrid.moveCurrentIndexUp();

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Items Selections
            browserGridRoot.setItemSelections(lastSelectionIndex, mainViewController.currentIndex + 1);
        }
    }

    // On Down Pressed
    Keys.onDownPressed: {
        //console.log("browserGridRoot.Keys.onDownPressed");

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Last Selection Index
            lastSelectionIndex = mainViewController.currentIndex;
        }

        // Move Current Index To Down
        browserGrid.moveCurrentIndexDown();

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Items Selections
            browserGridRoot.setItemSelections(lastSelectionIndex, mainViewController.currentIndex - 1);
        }
    }

    // On Left Pressed
    Keys.onLeftPressed: {
        //console.log("browserGridRoot.Keys.onLeftPressed");

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Last Selection Index
            lastSelectionIndex = mainViewController.currentIndex;
        }

        // Move Current Index To Left
        browserGrid.moveCurrentIndexLeft();

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Items Selections
            browserGridRoot.setItemSelections(lastSelectionIndex, mainViewController.currentIndex + 1);
        }
    }

    // On Right Pressed
    Keys.onRightPressed: {
        //console.log("browserGridRoot.Keys.onRightPressed");

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Last Selection Index
            lastSelectionIndex = mainViewController.currentIndex;
        }

        // Move Current Index To Right
        browserGrid.moveCurrentIndexRight();

        // Check Selection Mode
        if (browserGridRoot.selectionMode) {
            // Set Items Selections
            browserGridRoot.setItemSelections(lastSelectionIndex, mainViewController.currentIndex - 1);
        }
    }

    // On Return Pressed
    Keys.onReturnPressed: {
        //console.log("browserGridRoot.Keys.onReturnPressed");

        // Launch Viewer
        mainViewController.launchViewer();

        // ...
    }

    // On Escape Pressed
    Keys.onEscapePressed: {
        //console.log("browserGridRoot.Keys.onEscapePressed");

        // Hide Popup Menu
        hidePopupMenu();
    }

    // On Key Pressed``
    Keys.onPressed: {
        //console.log("browserGridRoot.Keys.onPressed - nativeScanCode: " + event.nativeScanCode);

        // Check Auto Repeat
        if (!event.isAutoRepeat) {

            // Check Shift Key
            if (event.nativeScanCode === 0) {
                // Shift Key - Selection Mode
                if (event.modifiers & Qt.ShiftModifier) {
                    //console.log("browserGridRoot.Keys.onPressed - SHIFT");

                    // Check Selection Mode
                    if (!browserGridRoot.selectionMode) {
                        // Set Selection Mode
                        browserGridRoot.selectionMode = true;

                        // Set Highlight Follow Current selection
                        //browserGrid.highlightFollowsCurrentItem = false;

                        // ...
                    }
                }

                // Meta Key - Single Selection Mode
                if (event.modifiers & Qt.ControlModifier) {
                    //console.log("browserGridRoot.Keys.onPressed - META");

                    // Check Single Selection Mode
                    if (!browserGridRoot.singleSelectionMode) {
                        // Set Single Selection Mode
                        browserGridRoot.singleSelectionMode = true;

                        // Set Highlight Follow Current selection
                        //browserGrid.highlightFollowsCurrentItem = false;
                    }
                }
            }

        } else {
            // Switch Key
            switch (event.key) {
                default:
                break;

                case Qt.Key_PageUp: {
                    console.log("browserGridRoot.Key.onReleased - PAGEUP");
                    // Calculate Visible Rows Count
                    var visibleRowCount = Math.floor(browserGrid.height / mainViewController.thumbsHeight);
                    // Loop
                    for (var i=0; i<visibleRowCount; i++) {
                        // Move Index Up
                        browserGrid.moveCurrentIndexUp();
                    }

                    // Position View
                    browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.Beginning);

                } break;

                case Qt.Key_PageDown: {
                    console.log("browserGridRoot.Key.onReleased - PAGEDOWN");

                    // Calculate Visible Rows Count
                    var visibleRowCount = Math.floor(browserGrid.height / mainViewController.thumbsHeight);
                    // Loop
                    for (var i=0; i<visibleRowCount; i++) {
                        // Move Index Down
                        browserGrid.moveCurrentIndexDown();
                    }

                    // Position View
                    browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.End);

                } break;
            }
        }

        // ...
    }

    // On Key Released
    Keys.onReleased: {
        //console.log("browserGridRoot.Keys.onReleased - nativeScanCode: " + event.nativeScanCode);

        // Check Shift Key
        if (event.nativeScanCode === 0) {
            // Shift Key - Selection Mode
            if (!(event.modifiers  & Qt.ShiftModifier)) {
                //console.log("browserGridRoot.Keys.onReleased - SHIFT");

                // Check Selection Mode
                if (browserGridRoot.selectionMode) {
                    // Reset Selection Mode
                    browserGridRoot.selectionMode = false;

                    // Set Highlight Follow Current selection
                    //browserGrid.highlightFollowsCurrentItem = true;
                }

                // ...
            }

            // Meta Key - Single Selection Mode
            if (!(event.modifiers  & Qt.ControlModifier)) {
                //console.log("browserGridRoot.Keys.onReleased - META");

                // Check Single Selection Mode
                if (browserGridRoot.singleSelectionMode) {
                    // Reset Single Selection Mode
                    browserGridRoot.singleSelectionMode = false;

                    // Set Highlight Follow Current selection
                    //browserGrid.highlightFollowsCurrentItem = true;
                }

                // ...
            }
        } else {

            // Switch Key
            switch (event.key) {
                case Qt.Key_Home:
                    console.log("browserGridRoot.Key.onReleased - HOME");
                    // Set Current Index
                    browserGrid.currentIndex = 0;
                    // Position View
                    browserGrid.positionViewAtBeginning();
                break;

                case Qt.Key_End:
                    console.log("browserGridRoot.Key.onReleased - END");
                    // Set Current Index
                    browserGrid.currentIndex = browserGrid.count - 1;
                    // Position View
                    browserGrid.positionViewAtEnd();
                break;

                case Qt.Key_PageUp: {
                    console.log("browserGridRoot.Key.onReleased - PAGEUP");
                    // Calculate Visible Rows Count
                    var visibleRowCount = Math.floor(browserGrid.height / mainViewController.thumbsHeight);
                    // Loop
                    for (var i=0; i<visibleRowCount; i++) {
                        // Move Index Up
                        browserGrid.moveCurrentIndexUp();
                    }

                    // Position View
                    browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.Beginning);

                } break;

                case Qt.Key_PageDown: {
                    console.log("browserGridRoot.Key.onReleased - PAGEDOWN");

                    // Calculate Visible Rows Count
                    var visibleRowCount = Math.floor(browserGrid.height / mainViewController.thumbsHeight);
                    // Loop
                    for (var i=0; i<visibleRowCount; i++) {
                        // Move Index Down
                        browserGrid.moveCurrentIndexDown();
                    }

                    // Position View
                    browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.End);

                } break;

                default:
                break;
            }

        }

        // ...
    }

    // Connections
    Connections {
        target: mainViewController
        // On Current Index Changed
        onCurrentIndexChanged: {
            //console.log("browserGridRoot.Connections.mainViewController.onCurrentIndexChanged - currentIndex: " + mainViewController.currentIndex);
            // Set Current Index
            browserGrid.currentIndex = mainViewController.currentIndex;
            // Position View At Index
            //browserGrid.positionViewAtIndex(browserGrid.currentIndex, GridView.End);
        }

        // On Deleting Index
        onDeletingIndex: {
            //console.log("browserGridRoot.Connections.mainViewController.onIndexDeleted - aIndex: " + aIndex);

            // Reset Highlight Follows Current Index
            browserGrid.highlightFollowsCurrentItem = false;
            // Reset Highlight Move Duration
            browserGrid.highlightMoveDuration = 0;
        }

        // On Index Deleted
        onIndexDeleted: {
            //console.log("browserGridRoot.Connections.mainViewController.onIndexDeleted - aIndex: " + aIndex);

            //console.log("browserGridRoot.Connections.mainViewController.onIndexDeleted - length: " + mainViewDataModel.length);

            // Reset Highlight Follows Current Index
            browserGrid.highlightFollowsCurrentItem = true;
            // Set Highlight Move Duration
            browserGrid.highlightMoveDuration = Const.defaultHighlightSpeed;

            // Update
            browserGrid.update();

            // ...
        }

        // On Browser Widget Focus Changed
        onBrowserFocusChanged: {
            //console.log("browserGridRoot.Connections.mainViewController.onBrowserFocusChanged - aFocused: " + aFocused);

            // Check Focused
            if (aFocused) {
                // Set Has Focus
                browserGridRoot.hasFocus = true;
            } else {
                // Reset Has Focus
                browserGridRoot.hasFocus = false;

                // Hide Popup Menu
                hidePopupMenu();
            }
        }

        // On Browser Mouse Pressed
        onBrowserMousePressed: {
            //console.log("browserGridRoot.Connections.mainViewController.onBrowserMousePressed - pos:[" + aPosX + ":" + aPosY + "] - aButton: " + aButton);

            // Check If Over Popup
            if (!browserGridRoot.mouseOverPopup) {
                // Hide Popup Menu
                hidePopupMenu();
            }
        }

        // On Browser Mouse Released
        onBrowserMouseReleased: {
            //console.log("browserGridRoot.Connections.mainViewController.onBrowserMouseReleased - pos:[" + aPosX + ":" + aPosY + "] - aButton: " + aButton);
            // Calculate Popup Pos X
            browserGridRoot.popupPosX = Math.min(aPosX, browserGridRoot.width - popupMenu.width);
            // Calculate Popup Pos Y
            browserGridRoot.popupPosY = Math.min(aPosY, browserGridRoot.height - popupMenu.height);

            // Check Button
            if (aButton === Qt.RightButton && browserGridRoot.delegateHasPointer && browserGridRoot.hoverIndex != -1) {
                console.log("browserGridRoot.Connections.mainViewController.onBrowserMouseReleased - pos:[" + aPosX + ":" + aPosY + "] - aButton: " + aButton + " - hoverIndex: " + browserGridRoot.hoverIndex);

                // Set Popup Index
                mainViewController.popupIndex = browserGridRoot.hoverIndex;

                // Show Popup
                showPopupMenu();
            }
        }
    }
}

