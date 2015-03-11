import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: viewerRoot

    property bool updateFlag: false

    width: 600
    height: 400

    //color: Const.defaultBackgroundColor
    color: Const.defaultViewBackgroundColor

/*
    // Viewer Grid Container
    Rectangle {
        id: viewerGridContainer

        anchors.fill: parent

        color: "transparent"

        // Viewer Grid
        GridView {
            id: viewGrid
            anchors.fill: parent
            cellWidth: parent.width
            cellHeight: parent.height
            snapMode: GridView.SnapPosition
            flow: GridView.FlowTopToBottom
            //highlightFollowsCurrentItem: true

            focus: true

            // Model
            model: mainViewDataModel

            // Delegate
            delegate: Rectangle {
                width: viewerGridContainer.width
                height: viewerGridContainer.height

                // Image
                Image {
                    id: viewImage
                    anchors.fill: parent
                    source: "file://" + mainViewController.currentDir + "/" + fileName
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                    cache: true
                }
            }

            // On Current Index Changed
            onCurrentIndexChanged: {
                // Update Main View Controller Current Index
                mainViewController.currentIndex = viewGrid.currentIndex;
            }

            // On Return Pressed
            Keys.onReturnPressed: {
                console.log("viewGrid.Keys.onReturnPressed");

                // ...
            }

            // On Escape Pressed
            Keys.onEscapePressed: {
                console.log("viewGrid.Keys.onEscapePressed");

                // ...
            }
        }
    }
*/
    // Connections
    Connections {
        target: mainViewController
        // On Current Index Changed
        onCurrentIndexChanged: {
            //console.log("viewerRoot.Connections.mainViewController.onCurrentIndexChanged - currentIndex: " + mainViewController.currentIndex);

            // Set Viewer Grid Current Index
            //viewGrid.currentIndex = mainViewController.currentIndex;
            // Position View At Index
            //viewGrid.positionViewAtIndex(viewGrid.currentIndex, GridView.Contain);

            // ...
        }

        // On Current File Changed
        onCurrentFileChanged: {
            //console.log("viewerRoot.Connections.mainViewController.onCurrentFileChanged - currentFile: " + mainViewController.currentFile);

            // Set Source
            viewerImage.source = "file://" + mainViewController.currentFile;

            // Show Status Text
            viewerViewController.showStatusText(mainViewController.currentFile, Const.defaultStatusTextDisplayTimeout);
        }

        // On Current File Updated
        onCurrentFileUdated: {
            //console.log("viewerRoot.Connections.mainViewController.onCurrentFileUpdated");
            // Set Update Flag
            viewerRoot.updateFlag = true;
            // Reset Preview Image
            viewerImage.source = "";
        }

        // On File Updated
        onFileUpdated: {
            //console.log("viewerRoot.Connections.mainViewController.onFileUpdated - aIndex: " + aIndex);
            // Check Index
            if (aIndex === mainViewController.currentIndex) {
                // Set Update Flag
                viewerRoot.updateFlag = true;
                // Reset Preview Image
                viewerImage.source = "";
            }
        }
    }

    focus: true

    // Viewer Image
    Image {
        id: viewerImage
        anchors.fill: parent
        anchors.margins: Const.defaultThumbMargins
        fillMode: Image.PreserveAspectFit
        cache: false
        asynchronous: true
        visible: opacity > 0.0
        opacity: viewerImage.status === Image.Ready ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }

        source: "file://" + mainViewController.currentFile

        // Mouse Area
        MouseArea {
            id: viewerMouseArea
            anchors.fill: parent
            // On Double Click
            onDoubleClicked: {
                // Toggle Viewer FullScreen
                mainViewController.toggleViewerFullScreen();
            }
            // On Wheel
            onWheel: {
                //console.log("viewerMouseArea.onWheel - wheel: " + wheel.angleDelta.y);
                // Check Angle Delta
                if (wheel.angleDelta.y < 0) {
                    // Next Image
                    mainViewController.nextImage();
                } else {
                    // Prev Image
                    mainViewController.prevImage();
                }

                // ...
            }
        }

        // On Status Changed
        onStatusChanged: {
            // Check Status
            if (viewerImage.status === Image.Null && viewerRoot.updateFlag) {
                //console.log("viewerImage.onStatusChanged - status: " + previewImage.status);

                // Reset Update Flag
                viewerRoot.updateFlag = false;
                // Set Source
                viewerImage.source = "file://" + mainViewController.currentFile;
            }
        }
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
        opacity: viewerImage.status !== Image.Ready ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }

        text: "Loading..."
    }

    // On Excape Pressed
    Keys.onEscapePressed: {
        //console.log("viewerRoot.Keys.onEscapePressed");

        // Hide Viewer
        mainViewController.hideViewer();
    }

    // On Enter Pressed
    Keys.onReturnPressed: {
        //console.log("viewerRoot.Keys.onReturnPressed");

        // Toggle Viewer FullScreen
        mainViewController.toggleViewerFullScreen();
    }

    // On Key Left Pressed
    Keys.onLeftPressed: {
        //console.log("viewerRoot.Keys.onLeftPressed");

        // Prev Image
        mainViewController.prevImage();
    }

    // On Key Right Pressed
    Keys.onRightPressed: {
        //console.log("viewerRoot.Keys.onRightPressed");

        // Next Image
        mainViewController.nextImage();
    }

    // On Key Space Pressed
    Keys.onSpacePressed: {
        //console.log("viewerRoot.Keys.onSpacePressed");

        // Next Image
        mainViewController.nextImage();
    }

    // On Key Up Pressed
    Keys.onUpPressed: {
        //console.log("viewerRoot.Keys.onUpPressed");

        // ...
    }

    // On Key Down Pressed
    Keys.onDownPressed: {
        //console.log("viewerRoot.Keys.onDownPressed");

        // ...
    }
}

