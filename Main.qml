import QtQuick
import QtQuick.Window
import QtQuick.Controls
import Painter
import QtQuick.Layouts

Window {
    width: 960
    height: 480
    visible: true
    title: qsTr("Webcam Viewer")
    color: "lightblue"

    Rectangle{
        id: imageRect
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.right: parent.right
        anchors.rightMargin: 50
        width: 400
        height: 300

        color: "transparent"
        border.color: "black"
        border.width: 3

        PaintItem {
            id: inputView
            streamerThreadSlot: VideoStreamer
            width: parent.width
            height: parent.height
        }
    }

    Text{
        text: qsTr("Camera Setting")
        anchors.bottom: cameraRect.top
        anchors.left: cameraRect.left
        anchors.margins: 5
    }

    Rectangle{
        id: cameraRect
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 50
        width: 200
        height: 50
        border.color: "black"
        border.width: 3
        ColumnLayout{
            anchors.centerIn: parent
            RowLayout{
                spacing: 10
                Button{
                    id: startButton
                    text: "Open"
                    width: 100
                    height: 45
                    onClicked: {
                        VideoStreamer.openVideoCamera(videoPath.text)
                        // opencvImage.visible = true
                    }
                }
                TextField{
                    id:videoPath
                    placeholderText: "Video Path or Video Index"
                    cursorVisible: true
                    height: startButton.height
                    width: startButton.width * 3
                }
            }
        }
    }

    Text{
        text: qsTr("Canny Edge Detection Setting")
        anchors.bottom: cannyRect.top
        anchors.left: cannyRect.left
        anchors.margins: 5
    }

    Rectangle{
        id: cannyRect
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: cameraRect.bottom
        anchors.topMargin: 50
        width: 200
        height: 75
        border.color: "black"
        border.width: 3
        ColumnLayout{
            anchors.centerIn: parent
            RowLayout{
                Text{
                    text: qsTr("Canny Enabled:")
                }
                CheckBox{
                    checked: VideoStreamer.cannyEnabled
                    onCheckedChanged: VideoStreamer.cannyEnabled = checked
                }
            }

            RowLayout{
                spacing: 10
                Text{
                    text: qsTr("Threshold 1")
                    width: 80
                }
                Text{
                    text: qsTr("Threshold 2")
                    width: 80
                }
            }
            RowLayout{
                spacing: 10
                TextField{
                    placeholderText: "Canny Threshold 1"
                    cursorVisible: true
                    implicitWidth: 80
                    text: VideoStreamer && VideoStreamer.cannyThreshold1
                    onTextChanged: VideoStreamer.cannyThreshold1 = text
                }
                TextField{
                    placeholderText: "Canny Threshold 2"
                    cursorVisible: true
                    implicitWidth: 80
                    text: VideoStreamer && VideoStreamer.cannyThreshold2
                    onTextChanged: VideoStreamer.cannyThreshold2 = text
                }
            }
        }
    }

}
