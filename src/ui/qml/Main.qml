import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts

ApplicationWindow {
  visible: true; width: 1100; height: 700; title: "deepThonk3d — Therapist MVP"

  Component.onCompleted: {
    bridge.rogerianReply.connect(function(text) {
      log.append("Therapist: " + text + "\n")
    })
  }

  menuBar: MenuBar {
    Menu { title: qsTr("Language")
      Action { text: "Português (pt-BR)"; onTriggered: bridge.setLocale("pt-BR") }
      Action { text: "English (en-US)";  onTriggered: bridge.setLocale("en-US") }
    }
  }

  RowLayout {
    anchors.fill: parent; spacing: 10; property int padding: 10

    // Left: Chat
    ColumnLayout {
      Layout.fillHeight: true; Layout.preferredWidth: parent.width * 0.60; spacing: 8
      TextArea {
        id: log; readOnly: true; wrapMode: TextEdit.WrapAnywhere
        Layout.fillWidth: true; Layout.fillHeight: true
      }
      RowLayout {
        TextField {
          id: prompt; placeholderText: qsTr("Share what's on your mind…")
          Layout.fillWidth: true
          onAccepted: send()
        }
        Button { text: qsTr("Send"); onClicked: send() }
      }
    }

    // Right: Therapist Structure
    ColumnLayout {
      Layout.fillHeight: true; Layout.preferredWidth: parent.width * 0.40; spacing: 8
      Label { text: qsTr("Therapist Structure"); font.pixelSize: 16 }
      TableView {
        id: ruleTableView
        Layout.fillWidth: true; Layout.fillHeight: true
        Component.onCompleted: function() {
          ruleTableView.model = bridge.ruleModel
        }
        delegate: RowLayout {
            width: parent.width
            spacing: 10
            Text {
                text: model.name
                width: 150
            }
            Text {
                text: model.hits
                width: 50
            }
        }
      }
    }
  }

  function send() {
    if (!prompt.text.length) return
    bridge.submitMessage(prompt.text)           // C++ slot
    log.append("You: " + prompt.text + "\n")
    prompt.text = ""
  }
}
