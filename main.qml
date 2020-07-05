import QtQuick 2.15
import QtQuick.Window 2.15
import Lobby 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    ListModel{
        id:appsModel
    }

    ListView{
        anchors.fill: parent
        model: appsModel
        delegate: Rectangle{
            width: 100; height: 30
            Text {
                anchors.fill: parent
                text: applicationPid + " " + localHostName
                MouseArea{
                    anchors.fill: parent
                    onClicked: lobby.connectApp(JSON.stringify({"tcpPort":tcpPort,"ipList":ipList}))
                }
            }
        }
    }

    Lobby{
        id:lobby
        onAppsChanged: {
            appsModel.clear()
            for (var prop in apps) {
                appsModel.append(JSON.parse(prop));
                console.log(apps)
            }
        }
        onMsgReceived: console.log(msg)
        onConnectedToServer: sendMsg("test")
    }
}
