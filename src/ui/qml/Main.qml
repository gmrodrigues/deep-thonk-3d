import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts
import com.deepthonk 1.0

ApplicationWindow {
  visible: true; width: 1100; height: 700; title: "deepThonk3d — Therapist MVP"

  Bridge {
    id: bridge
    onRogerianReply: (text, ruleId) => {
      log.append("Therapist [" + ruleId + "]: " + text + "\n")
    }
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
      TreeView {
        id: ruleTreeView
        Layout.fillWidth: true; Layout.fillHeight: true
        model: bridge.ruleModel

        delegate: RowLayout {
            width: parent.width

            Text {
                text: model.name
                Layout.preferredWidth: 150
            }
            Text {
                text: model.category
                Layout.preferredWidth: 200
            }
            Text {
                text: model.templates
                Layout.preferredWidth: 80
            }
            Text {
                text: model.hits
                Layout.preferredWidth: 50
            }
        }
      }
    }
  }

  function send() {
    if (!prompt.text.length) return    
    log.append("You: " + prompt.text + "\n")
    bridge.submitMessage(prompt.text)           // C++ slot
    prompt.text = ""
  }
}
