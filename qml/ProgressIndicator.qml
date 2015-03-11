import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: progressIndicatorRoot

    property int total: 0

    property int progress: 0

    property int progressBarFullWidth: width - 8


    width: Const.defaultProgressIndicatorWidth
    height: Const.defaultProgressIndicatorHeight

    color: Const.defaultPopupBackgroundColor

    //border.width: 2
    //border.color: Const.defaultPopupBorderColor

    radius: 4

    visible: opacity > 0.0

    Behavior on opacity {
        NumberAnimation {
            duration: 200
        }
    }

    // Hide Timer
    Timer {
        id: hideTimer
        interval: 1000
        repeat: false
        onTriggered: {
            // Set Opacity
            opacity = 0.0;
        }
    }

    // Progress Bar
    Rectangle {
        id: progressBar
        anchors.left: parent.left
        anchors.leftMargin: (parent.width - progressIndicatorRoot.progressBarFullWidth) / 2
        anchors.verticalCenter: parent.verticalCenter

        width: progressIndicatorRoot.progressBarFullWidth * progressIndicatorRoot.progress / progressIndicatorRoot.total
        height: Const.defaultProgressBarHeight

        color: Const.defaultHighlightColor

        //border.width: 1
        //border.color: Const.defaultFontColor



    }

    // On Completed
    Component.onCompleted: {
        //console.log("progressIndicatorRoot.onCompleted");

        // ...

    }

    // On Destruction
    Component.onDestruction: {
        //console.log("progressIndicatorRoot.onCompleted");

        // ...

    }

    // Connections
    Connections {
        target: mainViewController

        // On Progress Changed
        onProgressChanged: {
            //console.log("progressIndicatorRoot.Connections.mainViewController.onProgressChanged - aTotal: " + aTotal + " - aProgress: " + aProgress);

            // Set Opacity
            progressIndicatorRoot.opacity = 1.0;

            // Restart Hide Timer
            hideTimer.restart();

            // Set Total
            progressIndicatorRoot.total = aTotal;

            // Set Progress
            progressIndicatorRoot.progress = aProgress;

            // ...
        }
    }



}

