import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Lobby 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")
    property string connectedTo: ""

    ListModel{
        id:appsModel
    }

    ListModel{
        id:msgReceived
    }

    Column{
        y:5
        x:5
        spacing: 5
        Row{
            spacing: 5
            Rectangle{
                width: 150
                height: 100
                border.width: 1
                ListView{
                    anchors.fill: parent
                    anchors.margins: 2
                    model: appsModel
                    delegate: Rectangle{
                        width: 100; height: 30
                        Text {
                            anchors.fill: parent
                            text: applicationPid + " " + localHostName
                            font.bold: connectedTo.indexOf(applicationPid + " " + localHostName)>=0
                            MouseArea{
                                anchors.fill: parent
                                onClicked: lobby.connectApp(JSON.stringify({"applicationPid":applicationPid,"localHostName":localHostName,"tcpPort":tcpPort,"ipList":ipList}))
                            }
                        }
                    }
                }
            }
            Rectangle{
                width: 150
                height: 100
                border.width: 1
                ListView{
                    anchors.fill: parent
                    anchors.margins: 2
                    model: msgReceived
                    highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
                    delegate: Rectangle{
                        width: 100; height: 30
                        Text {
                            anchors.fill: parent
                            text: msg
                        }
                    }
                }
            }
        }

        Button{
            text: "send test"
            onClicked: lobby.sendMsg("test")
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
        onMsgReceived: {console.log(msg)
            msgReceived.append({"msg":msg})
        }
        onConnectedToServer: {sendMsg("test")
            console.log("server:"+server)
            var json = JSON.parse(server)
            connectedTo+=json.applicationPid + " " + json.localHostName
        }
    }
}
