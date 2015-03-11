import QtQuick 2.0
import "qrc:/qml/js/constants.js" as Const

Rectangle {
    id: duplcateListRoot

    //color: Const.defaultBackgroundColor
    //color: "transparent"
    color: "#FFEEEEEE"

    ListView {
        id: duplcateList

        anchors.fill: parent

        // Model
        model: duplicateListModel

        spacing: 2

        // Delegate
        delegate: Rectangle {
            id: delegateRoot
            color: "transparent"
            width: duplcateList.width
            height: Math.max(imageList.length * Const.duplicateFileListItemHeight, Const.duplicateListImageHeight + border.width * 2);

            border.color: Const.defaultBorderColor
            border.width: 1

            Row {
                anchors.fill: parent

                // File List Column
                Column {
                    id: delegateFileListColumn
                    width: duplcateList.width - Const.duplicateListImageWidth
                    // Duplicate File List Repeater
                    Repeater {
                        id: delegateFileList
                        // Model
                        model: imageList

                        // Repeater Delegate
                        delegate: Rectangle {
                            color: "transparent"
                            width: delegateFileListColumn.width
                            height: Const.duplicateFileListItemHeight

                            // File Name
                            Text {
                                anchors.fill: parent
                                anchors.rightMargin: height
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideMiddle
                                text: modelData.substr(modelData.length - 2, 2) === " *" ? modelData.substr(0, modelData.length - 2) : modelData
                            }

                            // Check Box
                            Rectangle {
                                id: delegateFileListCheckBox

                                property bool checked: modelData.substr(modelData.length - 2, 2) === " *"

                                height: parent.height - 2
                                width: height
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                //color: "#44000044"
                                color: "transparent"
                                border.width: 1
                                border.color: Const.defaultBorderColor

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        // Get Temp List
                                        var tempList = imageList;
                                        // Get Item
                                        var item = tempList[index];

                                        // Check Item
                                        if (item.substr(modelData.length - 2, 2) === " *") {
                                            // Set Item
                                            item = item.substr(0, modelData.length - 2);
                                        } else {
                                            // Set Item
                                            item = item + " *";
                                        }

                                        console.log("delegateFileListCheckBox.onClicked - item: " + item);

                                        // Set Temp List
                                        tempList[index] = item;

                                        // Set Image List
                                        imageList = tempList;
                                    }
                                }

                                Rectangle {
                                    anchors.fill: parent
                                    anchors.margins: 2
                                    color: Const.defaultBorderColor
                                    opacity: parent.checked ? 1.0 : 0.0
                                    visible: opacity > 0.0
                                    Behavior on opacity { NumberAnimation { duration: 50 } }
                                }
                            }
                        }
                    }
                }

                // Duplicate File Image Sample
                Image {
                    id: delegateFileImage
                    width: Const.duplicateListImageWidth
                    height: Const.duplicateListImageHeight
                    anchors.verticalCenter: parent.verticalCenter
                    fillMode: Image.PreserveAspectFit
                    asynchronous: true
                    source: "file://" + imageSource
                }
            }
        }
    }
}

