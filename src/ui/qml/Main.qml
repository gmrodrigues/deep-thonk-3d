import QtQuick
import QtQuick.Controls 2.15
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
        id: chatLog; readOnly: true; wrapMode: TextEdit.WrapAnywhere
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
      RowLayout {
        Text { text: "ID"; Layout.preferredWidth: 100; font.bold: true }
        Text { text: "Category"; Layout.preferredWidth: 80; font.bold: true }
        Text { text: "Pattern"; Layout.preferredWidth: 120; font.bold: true }
        Text { text: "Templates"; Layout.preferredWidth: 70; font.bold: true; horizontalAlignment: Text.AlignHCenter }
        Text { text: "Hits"; Layout.preferredWidth: 50; font.bold: true; horizontalAlignment: Text.AlignHCenter }
      }
      TreeView {
        id: ruleTreeView
        Layout.fillWidth: true; Layout.fillHeight: true
        model: bridge.ruleModel

        delegate: RowLayout {
            width: parent.width

            Text {
                text: model.name
                Layout.preferredWidth: 100
                elide: Text.ElideRight
            }
            Text {
                text: model.category
                Layout.preferredWidth: 80
                elide: Text.ElideRight
            }
            Text {
                text: model.pattern
                Layout.preferredWidth: 120
                elide: Text.ElideRight
            }
            Text {
                text: model.templates
                Layout.preferredWidth: 70
                horizontalAlignment: Text.AlignHCenter
            }
            Text {
                text: model.hits
                Layout.preferredWidth: 50
                horizontalAlignment: Text.AlignHCenter
            }
        }
      }
    }
  }

  Connections {
    target: bridge
    function onRogerianReply(reply, ruleId) {
      const timestamp = new Date().toLocaleTimeString();
      chatLog.text += `[${timestamp}] Therapist: ${reply} (rule: ${ruleId})\n`;
    }
  }

  function send() {
    if (!prompt.text.length) return
    const time = new Date().toLocaleTimeString();
    chatLog.append("[" + time + "] You: " + prompt.text + "\n")
    bridge.submitMessage(prompt.text)
    prompt.text = ""
  }
}
