import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: previewRoot

    width: 400
    height: 300

    color: Const.defaultBackgroundColor;

    property bool updateFlag: false

    property string fileInfo: previewImage.sourceSize.width + "x" + previewImage.sourceSize.height + " size: " + mainViewController.getFileSize(mainViewController.currentFile)

    // Preview Image
    Image {
        id: previewImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        asynchronous: true
        //cache: true
        cache: false
        visible: opacity > 0.0
        opacity: previewImage.status === Image.Ready ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }

        // Preview Image Source
        source: "file://" + mainViewController.currentFile

        // On Status Changed
        onStatusChanged: {
            // Check Status
            if (previewImage.status === Image.Null && previewRoot.updateFlag) {
                //console.log("previewImage.onStatusChanged - status: " + previewImage.status);

                // Reset Update Flag
                previewRoot.updateFlag = false;
                // Set Source
                previewImage.source = "file://" + mainViewController.currentFile;
                // Reset File Info
                previewRoot.fileInfo = "";

            // Check Status
            } else if (previewImage.status === Image.Ready) {
                // Set File Info
                previewRoot.fileInfo = previewImage.sourceSize.width + "x" + previewImage.sourceSize.height + " size: " + mainViewController.getFileSize(mainViewController.currentFile);
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
        opacity: previewImage.status !== Image.Ready ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }

        text: mainViewController.currentFile.length > 1 ? previewImage.status === Image.Error ? "Error." : "Loading..." : ""
    }

    // Delegate Title Shadow
    Text {
        id: thumbTitleShadow
        width: thumbTitle.width
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
        elide: Text.ElideRight
        font.pixelSize: Const.defaultFontSize
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Const.defaultThumbMargins * 2
        anchors.horizontalCenter: parent.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        color: Const.defaultFontColor

        text: mainViewController.currentFile.length > 1 ? fileInfo : ""
    }

    // On Completed
    Component.onCompleted: {
        //console.log("previewRoot.onCompleted");

        // ...
    }

    // On Destruction
    Component.onDestruction: {
        //console.log("previewRoot.onDestruction");

        // ...
    }

    // Connections
    Connections {
        target: mainViewController

        // On Current File Updated
        onCurrentFileUdated: {
            //console.log("previewRoot.Connections.mainViewController.onCurrentFileUpdated");
            // Set Update Flag
            previewRoot.updateFlag = true;
            // Reset Preview Image
            previewImage.source = "";
        }

        // On File Updated
        onFileUpdated: {
            //console.log("previewRoot.Connections.mainViewController.onFileUpdated - aIndex: " + aIndex);
            // Check Index
            if (aIndex === mainViewController.currentIndex) {
                // Set Update Flag
                previewRoot.updateFlag = true;
                // Reset Preview Image
                previewImage.source = "";
            }
        }

        // On Current File Changed
        onCurrentFileChanged: {
            //console.log("previewRoot.Connections.mainViewController.onCurrentFileChanged - currentFile: " + mainViewController.currentFile);

            // Set Source
            previewImage.source = "file://" + mainViewController.currentFile;
        }
    }
}

