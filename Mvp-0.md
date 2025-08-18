# deepThonk3d — First Delivery (MVP-0)

## 1) Scope (and only this)

* **Rogerian Therapist** (rule-based ELIZA-style) with **Portuguese (pt-BR)** and **English (en-US)** packs.
* **UI** with three pieces:

  1. **Log window** (chat history, timestamped).
  2. **Prompt window** (single-line input + Send).
  3. **“Therapist Structure” panel**: a **TreeView** of the rule engine (Locale → Category → Rule), showing pattern, template count, and live hit counters (how often each rule matched).
* **Local-only**; no persistence required yet (optional: save chat as .txt).
* **No 3D map yet**.

## 2) Deliverables

* **Executable** for Desktop (Linux/macOS/Windows) and a **WASM build**.
* **Two rule packs** (pt-BR & en-US) as data files.
* **Minimal tests** (unit + golden tests for rule responses).
* **README snippet** (how to run, switch locale).
* **CMakePresets.json** for desktop + wasm.

## 3) UI & UX (QML)

Layout: split screen (left: log+prompt; right: structure).

```qml
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
    anchors.fill: parent; spacing: 10; padding: 10

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
```

**Behavior**

* After `submitMessage`, C++:

  1. picks the best-matching rule,
  2. applies pronoun **reflection**,
  3. chooses a response template,
  4. emits `rogerianReply(QString)`.
* QML appends: `Therapist: …` to the log.

## 4) Rule Engine (C++ core)

### Data structures

```cpp
enum class Locale { PT_BR, EN_US };
struct RuleTemplate { std::string text; };
struct Rule {
  std::string id;                 // "feelings.sense"
  std::string category;           // "Feelings", "Agency"…
  std::regex  pattern;            // compiled with icase, ECMAScript
  std::vector<RuleTemplate> outs; // responses
  uint64_t hits = 0;              // live counter
};
struct RulePack {
  Locale locale;
  std::vector<Rule> rules;
  // reflection mapping: "eu->você", "my->your", … (case-insensitive)
  std::vector<std::pair<std::string,std::string>> reflectPairs;
};
```

### Matching flow

```cpp
std::string Engine::respond(const std::string& userText) {
  const auto& rules = activePack.rules;
  const Rule* best = nullptr;
  std::smatch m;

  for (auto& r : rules) {
    if (std::regex_search(userText, m, r.pattern)) { best = &r; break; } // first-hit strategy MVP
  }
  if (!best) return pickNeutralProbe();

  std::string captured = m.size() > 1 ? m[1].str() : userText;
  std::string reflected = reflectPronouns(captured, activePack.reflectPairs);
  auto tpl = pickTemplate(best->outs);
  best->hits++;

  return substitutePlaceholders(tpl.text, reflected); // e.g. {1} -> reflected
}
```

### Example rule pack (YAML-ish; embed as JSON or YAML)

**en-US**

```yaml
locale: en-US
reflect:
  - [ "I", "you" ]            # handled case-insensitively
  - [ "my", "your" ]
  - [ "me", "you" ]
  - [ "am", "are" ]
rules:
  - id: feelings.sense
    category: Feelings
    pattern: "(?:I\\s+feel\\s+|I\\s+am\\s+feeling\\s+)(.+)"
    outs:
      - "When you say you feel {1}, what comes to mind now?"
      - "What is it like to feel {1} at this moment?"
  - id: agency.cannot
    category: Agency
    pattern: "I\\s+(?:can't|cannot|don’t|do not)\\s+(.+)"
    outs:
      - "What makes it difficult for you to {1}?"
      - "When has {1} felt more possible?"
  - id: fallback.any
    category: General
    pattern: "(.+)"
    outs:
      - "Say more about that."
      - "What stands out to you in this?"
```

**pt-BR**

```yaml
locale: pt-BR
reflect:
  - [ "eu", "você" ]
  - [ "meu", "seu" ]
  - [ "minha", "sua" ]
  - [ "mim", "você" ]
  - [ "sou", "é" ]
rules:
  - id: sentimentos.sinto
    category: Sentimentos
    pattern: "(?:eu\\s+sinto\\s+|tenho\\s+sentido\\s+)(.+)"
    outs:
      - "Quando você diz que sente {1}, o que vem à mente agora?"
      - "Como é para você sentir {1} neste momento?"
  - id: agencia.naoconsigo
    category: Agência
    pattern: "eu\\s+(?:nao|não)\\s+consigo\\s+(.+)"
    outs:
      - "O que torna difícil para você {1}?"
      - "Em que momentos {1} pareceu mais possível?"
  - id: fallback.qualquer
    category: Geral
    pattern: "(.+)"
    outs:
      - "Fale mais sobre isso."
      - "O que te chama atenção nisso agora?"
```

> Notes:
>
> * Use **Unicode** and `std::regex` with `icase`; normalize to lower-case before matching for PT accents, or prebuild **accent-insensitive** regex (e.g., `[aáàãâ]`).
> * First-delivery: **first-match wins**; later you can rank by specificity/priority.

## 5) “Structure” Panel (Tree Model)

Represent the active rule pack as a tree:

```
<Locale>
  ├─ Category "Sentimentos"
  │   ├─ Rule sentimentos.sinto  [pattern, templates=2, hits=14]
  │   └─ Rule …
  └─ Category "Geral"
      └─ Rule fallback.qualquer  [pattern, templates=2, hits=56]
```

* **Columns:** Node, Pattern (preview), Templates (count), Hits (live).
* **Live updates:** when a rule matches, increment hits and emit `dataChanged` to refresh the row.
* Useful for debugging AND as a “structural” representation of the therapist.

## 6) Internationalization

* **Locale switching:** Menu → `bridge.setLocale("pt-BR"|"en-US")` loads rule pack from resources.
* **UI strings:** wrap in `qsTr()` and provide `pt-BR` & `en-US` translations.
* **Reflection edge cases:** handle casing and multi-word pronouns (e.g., “da minha” → “da sua”). MVP can keep it simple; refine later.

## 7) Build & Packaging

**CMake options**

* `-DWEB=ON` for Emscripten/Qt WASM kit.
* `-DELIZA_ONLY=ON` to keep core tiny (no NLP-light).
* `-DLOCALE_PT=ON -DLOCALE_EN=ON` to embed both packs.

**Presets**

* `desktop-debug`, `desktop-release`, `wasm-release`.

**WASM notes**

* Keep dependencies **header-only**.
* Disable SQL/threads.
* Use basic fonts (Qt default).

## 8) Testing & Acceptance

**Unit tests**

* **Reflection:** input→output pairs for PT/EN (case & accent cases).
* **Rule matching:** each rule has 2–3 example utterances that should match.
* **Fallback safety:** any string returns non-empty reply.

**Golden tests**

* Deterministic seed → template rotation predictable; snapshot expected response.

**Manual acceptance checklist**

* [ ] Switch language menu changes responses instantly.
* [ ] Typing “I feel anxious about work.” triggers **feelings.sense**.
* [ ] “Eu não consigo dormir.” triggers **agencia.naoconsigo**.
* [ ] Structure panel shows **Hits** incrementing on each match.
* [ ] WASM build runs in browser; chat works in both languages.
* [ ] Log window scrolls; Enter sends; Button sends.

## 9) Risks & Mitigations

* **Accent/diacritics in PT:** normalize input (NFD strip marks) to avoid missing matches.
* **Regex brittleness:** keep patterns conservative; always include a safe fallback.
* **WASM memory:** keep assets tiny; no extra libs; avoid large fonts.

## 10) Minimal Directory Layout

```
/src
  /core/rogerian/           # Engine, RulePack loader, reflection
  /ui/bridge/               # QObject with slots/models
  /ui/qml/                  # Main.qml
/resources
  rules/en-US.yaml
  rules/pt-BR.yaml
/tests
  test_reflection.cpp
  test_rules.cpp
CMakeLists.txt
CMakePresets.json
```
