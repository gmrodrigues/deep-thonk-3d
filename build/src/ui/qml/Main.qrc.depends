import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  visible: true; width: 1100; height: 700; title: "deepThonk3d — Therapist MVP"
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
        function send() {
          if (!prompt.text.length) return
          bridge.submitMessage(prompt.text)           // C++ slot
          log.append("You: " + prompt.text + "\n")
          prompt.text = ""
        }
      }
    }

    // Right: Therapist Structure
    ColumnLayout {
      Layout.fillHeight: true; Layout.preferredWidth: parent.width * 0.40; spacing: 8
      Label { text: qsTr("Therapist Structure"); font.pixelSize: 16 }
      TreeView {
        Layout.fillWidth: true; Layout.fillHeight: true
        model: bridge.ruleModel                         // QAbstractItemModel
        TableViewColumn { role: "name"; title: qsTr("Node") }
        TableViewColumn { role: "pattern"; title: qsTr("Pattern") }
        TableViewColumn { role: "templates"; title: qsTr("Templates") }
        TableViewColumn { role: "hits"; title: qsTr("Hits") }
      }
    }
  }
}
