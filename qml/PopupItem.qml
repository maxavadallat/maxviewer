import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: popupItemRoot

    property alias title: titleLabel.text

    property int command: 0

    property bool mousePressed: false

    property bool mouseHovered: false

    width: Const.defaultPopupWidth - 4
    height: popupItemRoot.title === "-" ? Const.defaultPopupItemHeight * 0.5 : Const.defaultPopupItemHeight

    anchors.horizontalCenter: parent.horizontalCenter

    color: mouseHovered ? Const.defaultHighlightColorFocused : "transparent"

    signal itemClicked(var aCommand);

    // Title Label
    Text {
        id: titleLabel
        anchors.fill: parent
        anchors.leftMargin: Const.defaultThumbMargins
        anchors.rightMargin: Const.defaultThumbMargins
        verticalAlignment: Text.AlignVCenter
        visible: popupItemRoot.title !== "-"
        color: Const.defaultFontColor

    }

    // Separator
    Rectangle {
        id: separator
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        height: 1
        color: Const.defaultBorderColor
        visible: popupItemRoot.title === "-"
    }

    // Mouse Area
    MouseArea {
        anchors.fill: parent
        hoverEnabled: popupItemRoot.title !== "-"
        preventStealing: true

        // On Pressed
        onPressed: {
            // Set Accepted
            mouse.accepted = true;
            // Set Mouse Pressed
            popupItemRoot.mousePressed = true;
        }

        // On Clicked
        onReleased: {
            // Check Mouse Pressed
            if (mousePressed) {
                // Reset Mouse Pressed
                popupItemRoot.mousePressed = false;
                // Set Accepted
                mouse.accepted = true;
                // Emit Item Clicked Signal
                itemClicked(popupItemRoot.command);
            }
        }

        // On Entered
        onEntered: {
            // Set Color
            //popupItemRoot.color = Const.defaultHighlightColorFocused;
            // Check Title
            if (popupItemRoot.title !== "-") {
                // Set Mouse Hovered
                mouseHovered = true;
            }
        }

        // On Exited
        onExited: {
            // Reset Color
            //popupItemRoot.color = "transparent";
            // Reset Mouse Hovered
            mouseHovered = false;
        }
    }
}

