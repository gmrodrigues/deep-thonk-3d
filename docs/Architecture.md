# deepThonk3d — Architecture Definition

## 1) Product & Non-Functional Requirements

**Goal:** Rogerian chat (ELIZA-style) that builds a **live 3D mind-map** from the user’s text.
**Targets:** Local-first, offline by default, smooth on desktop, acceptable in WebAssembly.

**Key NFRs**

* **Performance:**

  * Desktop (Win/macOS/Linux): 3–5k nodes / 8–12k edges @ ≥45 FPS during local layout relax.
  * WebAssembly: 1–2k nodes / 3–6k edges @ ≥30 FPS.
* **Startup:** <2s desktop, <5s WASM (cold).
* **Privacy:** no network by default; readable local export.
* **Stability:** graceful degradation in WASM (no SQLite, no threads unless supported).
* **Portability:** Single codebase for desktop + WASM.

---

## 2) Tech Stack & Tooling

### Languages & Runtimes

* **C++20** for core engine (portable, perf-friendly).
* **Qt 6.7+ / QML / Qt Quick 3D** for UI & rendering.
* **Qt for WebAssembly** for browser target (Emscripten).
* **CMake** (3.26+) as build system.

### Compilers

* **Desktop:** Clang 16+/GCC 13+/MSVC 19.3+.
* **WASM:** **Emscripten 3.1.x** (match Qt WASM kit).

### Third-Party (header-only where possible)

* **Math/containers:**

  * `glm` (MIT) — vec/mat/quats for layout numerics.
  * `ankerl::unordered_dense` (MIT) — fast hash maps/sets.
* **NLP light:**

  * Custom **RAKE** (PT/EN stopwords embedded).
  * **SimHash** (tiny impl) + **xxHash** (BSD) for fast hashing.
* **Serialization:**

  * `nlohmann::json` (MIT) for JSON save/load.
  * `tinyxml2` (zlib) for GraphML/GEXF export.
* **Logging & utils:**

  * `spdlog` (MIT) for structured logs.
  * `fmt` (MIT) already bundled via spdlog.
* **Testing:**

  * `Catch2 v3` (BSL) unit tests.
  * Optional **libFuzzer**/**AFL++** fuzz targets (desktop only).

> **Avoid Boost** to keep size/build time low, especially in WASM.

### Dev Tooling

* **clang-format**, **clang-tidy** (modernize, readability, perf).
* **cppcheck** (static).
* **sanitizers:** ASan/UBSan/TSan in debug presets.
* **CMake presets** (below).
* **GitHub Actions**: matrix (win/macos/linux + wasm), cache Qt & build artifacts.

---

## 3) High-Level Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                            QML / UI                            │
│  Chat Panel   Filters/Timeline    3D View (Qt Quick 3D)        │
│     │                 │                 │                      │
│     ▼                 ▼                 ▼                      │
│                    ui::bridge (C++/Qt) — QAbstractListModels   │
└───────────────▲───────────────────────────────────────▲────────┘
                │                                       │
        deepcore::rogerian                       deepcore::layout3d
                │                                       │
                ▼                                       ▼
         deepcore::nlp_light  ─────►  deepcore::graph_store  ◄─── storage
                │                               ▲
                └────────────── metrics/logging ┴───────────────► export
```

**Module boundaries**

* `deepcore::graph_store` — nodes/edges, indices, ops, serialization.
* `deepcore::nlp_light` — tokenization, RAKE, stopwords, simhash.
* `deepcore::rogerian` — regex rules, pronoun reflection, response picker.
* `deepcore::layout3d` — force-directed layout, octree Barnes–Hut, local relax.
* `deepcore::storage` — JSON for all; SQLite(FTS5) on desktop only.
* `ui::bridge` — C++/Qt models to QML (`NodeModel`, `EdgeModel`, commands).

**Threading model**

* **Desktop:** background worker thread(s) for NLP and layout; UI thread for rendering.
* **WASM:** single-threaded by default; optional PThreads build when COOP/COEP available.

---

## 4) Data Model

### Node / Edge

```cpp
enum class NodeKind : uint8_t { Concept, Statement, Question, Tag };
enum class EdgeKind : uint8_t { Related, Mentions, Supports, Contradicts, Causes };

struct Node {
  uint32_t id;
  NodeKind kind;
  std::string text;     // UTF-8 label (core uses std::string)
  float weight;         // importance/frequency
  uint64_t ts;          // epoch ms
  uint64_t hash;        // SimHash bucket for similarity
  glm::vec3 pos;        // layout position (authoritative in core)
  bool pinned{false};
};

struct Edge {
  uint32_t a, b;
  EdgeKind kind;
  float weight;
};
```

**Indices**

* by `id` (dense vector), by `hash` (bucketed), by time (ring buffer or vector), by degree (lazy cached).
* `unordered_dense_map` from ankerl for string→node id.

### JSON Save Format (simplified)

```json
{ "version": 1,
  "nodes":[{"id":1,"k":"Concept","t":"ansiedade","w":2.3,"ts":169...,"h":123,"p":[0.1,1.2,-0.6],"pin":false}],
  "edges":[{"a":1,"b":5,"k":"Related","w":0.7}],
  "meta":{"locale":"pt-BR","created":169...}
}
```

### GraphML/GEXF Export

* Include node attributes: `kind`, `weight`, `ts`, `community`, `pinned`.
* Edge attributes: `kind`, `weight`.

---

## 5) Core Algorithms

### NLP Light (on-device)

* **Tokenize:** Unicode letters/digits; lower-case; strip punctuation; PT/EN stopwords (embedded text resources).
* **Keyphrases:** **RAKE** with adjustable window (default 3).
* **Similarity:** **SimHash** over unigrams/bigrams; Hamming distance threshold to connect to existing concepts.
* **Edges:**

  * `Mentions`: Statement → Concept (same message).
  * `Related`: Concept ↔ Concept if co-occurrence or simhash within threshold.
  * Optional `Supports/Contradicts`: keyword heuristics later.

### Rogerian Engine

* Pattern rules (regex) + pronoun reflection table.
* Response selection: rule → template; insert reflected captures; fallback to neutral probes.
* Temperature-like variation: rotate templates per rule to avoid repetition.

### Layout 3D

* **Force-directed** with:

  * Repulsion: Coulomb (q=node weight).
  * Attraction: Hooke on edges.
  * Damping + cooling schedule.
* **Barnes–Hut Octree** for O(N log N) repulsion.
* **Local relax:** when a new node arrives, relax k-hops neighborhood for 20–60 iters.
* **Pinning:** pinned nodes excluded from force updates.
* **Collision softness:** tiny radius to avoid complete overlap without heavy physics.

---

## 6) Rendering & UI

### Qt Quick 3D

* **Nodes:** one `Model { source: "#Sphere"; instancing: InstanceList }` (instanced draw).
* **Edges:**

  * Mode A: cylinders as instanced models (simple, heavier).
  * Mode B (WASM default): custom `QQuick3DGeometry` **line list** (lighter).
* **Labels:** 2D overlay (`Text` in screen space on selection) to avoid overdraw.
* **Colors:** by community (Louvain/Leiden lite) or by kind.
* **Interaction:** orbit, pan, zoom, click to focus, highlight neighbors, pin/unpin, merge.

### QML Models (roles)

* `NodeModel` roles: `id`, `label`, `pos` (vec3), `color` (vec3), `scale`, `pinned`, `degree`, `community`.
* `EdgeModel` roles: `a`, `b`, `weight`, `kind`.

---

## 7) Storage & Persistence

* **Desktop**

  * Primary: JSON file per session; auto-save.
  * Optional: **SQLite + FTS5** for full-text search of statements. Use Qt SQL module; DB lives in app data dir.
* **WASM**

  * JSON only; persisted to **IndexedDB** via Emscripten **IDBFS** (Qt picks up the VFS).
  * Manual “Export/Import” (download/upload JSON).

---

## 8) Security & Privacy

* **Local-first:** no network by default; any future analytics opt-in, off by default.
* **Export readability:** human-readable JSON; user controls their data.
* **Optional encryption (desktop):** `libsodium` secretbox (XChaCha20-Poly1305) with password-derived key (scrypt).
* **WASM note:** crypto is best-effort; warn about weaker threat model in browsers.

---

## 9) Build, Packaging, CI

### CMake Presets (`CMakePresets.json`)

* `desktop-debug` — ASan/UBSan, `-O0 -g`, `-fsanitize=address,undefined`.
* `desktop-release` — `-O3 -DNDEBUG`, LTO, `-fno-exceptions` in hot paths (where safe).
* `wasm-release` — Qt WASM toolchain; `-O3`, `-sALLOW_MEMORY_GROWTH=1`; LineList edges.

**Options**

* `-DWEB=ON|OFF`
* `-DENABLE_SQLITE=ON|OFF` (OFF in WASM)
* `-DELIZA_ONLY=ON|OFF` (no NLP, just chat → for smoke tests)
* `-DNLP_ENHANCED=ON|OFF` (RAKE + SimHash)
* `-DENABLE_THREADS=ON|OFF` (WASM requires COOP/COEP)

### Packaging

* **Windows:** `windeployqt` + NSIS.
* **macOS:** `.app` bundle + `macdeployqt` + notarization.
* **Linux:** AppImage (linuxdeploy + qt plugin).
* **WASM:** HTML + JS glue + `.wasm` (single folder).

### CI (GitHub Actions)

* Build matrix, run unit tests, produce artifacts.
* Cache: ccache, Qt archives, Emscripten SDK.
* Lint job (clang-tidy), format check, sanitizer runs (linux).

---

## 10) Testing Strategy

* **Unit tests:** graph ops, RAKE extraction, simhash distance, Rogerian rules.
* **Property tests:** merging nodes, idempotent save/load, neighborhood relax invariants.
* **Golden tests:** prompt→response fixtures for Rogerian engine.
* **Fuzzing:** JSON loader, graph import, regex rule engine.
* **Perf baselines:** layout step time vs. N (1k, 2k, 5k nodes).

---

## 11) Observability & Diagnostics

* **In-app debug overlay:** FPS, node/edge counts, layout iterations, octree depth.
* **Logging levels:** TRACE (dev), INFO (default), WARN/ERROR.
* **Event markers:** “message\_received”, “nodes\_created”, “edges\_created”, “layout\_relax(ms)”.

---

## 12) Internationalization & Locale

* **Qt tr()** for all UI strings; `.ts` files for **pt-BR** and **en-US**.
* **NLP stopwords:** separate PT/EN lists; fallback n-grams if locale unknown.
* **Number/date formatting:** `QLocale`.

---

## 13) Public API (UI Bridge)

**Commands** (invoked from QML):

* `submitMessage(QString text)`
* `pinNode(uint32_t id, bool value)`
* `mergeNodes(uint32_t a, uint32_t b)`
* `addManualEdge(uint32_t a, uint32_t b, EdgeKind k)`
* `saveProject(QString path)` / `loadProject(QString path)`
* `exportGraphML(QString path)`
* `recomputeCommunities()`
* `replayToTimestamp(uint64_t ts)`

**Signals**

* `nodesUpdated()` / `edgesUpdated()`
* `rogerianReply(QString text)`
* `layoutProgress(int percent)` (desktop builds)

---

## 14) Directory Layout

```
/deepThonk3d
  /cmake
  /external            # header-only third-parties (vendored)
  /src
    /core
      graph_store/
      nlp_light/
      rogerian/
      layout3d/
      storage/
      utils/
    /ui
      bridge/          # NodeModel, EdgeModel, commands
      qml/             # QML/Qt Quick 3D, assets
  /resources
    stopwords/pt.txt
    stopwords/en.txt
    shaders/
  /tests
  /tools               # scripts: export, profiling, gen-stopwords
```

---

## 15) Performance Plan

* **Instancing:** single draw call for many nodes; lines over cylinders in WASM.
* **Memory pools:** reserve vectors/maps up-front; avoid per-frame allocations.
* **Partial updates:** only refresh changed instance transforms.
* **Local relax:** limit neighborhood radius & iterations per frame.
* **String handling:** store labels once; intern concepts; use ids in hot loops.
* **Hash buckets:** use simhash buckets for near-duplicate detection in O(1) avg.

---

## 16) Roadmap Hooks

* **Local LLM plugin (desktop):** interface via `QPluginLoader`; runtime disabled in WASM.
* **STT/TTS (desktop):** Coqui-TTS/Vosk as optional plugins.
* **Emotion tinting:** simple sentiment coloring (non-clinical).
* **Scripting:** Lua or QJSEngine for custom extractors/layouts (careful in WASM).

---

## 17) Risks & Mitigations

* **Visual clutter:** edge fading, bundling light, progressive disclosure, pinning.
* **WASM limitations:** no SQLite, avoid dynamic plugins; test IDBFS robustness.
* **Regex brittleness:** comprehensive rule tests; safe fallbacks.
* **International text:** normalization & robust tokenization; avoid ICU in WASM.

---

## 18) Example CMake Flags (WASM)

```cmake
set(CMAKE_BUILD_TYPE Release)
set(QT_FEATURE_sql OFF)                  # no SQL in WASM
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -sALLOW_MEMORY_GROWTH=1")
# Optional threads (needs COOP/COEP):
# -sUSE_PTHREADS=1 -sPTHREAD_POOL_SIZE=2 -sPROXY_TO_PTHREAD
```

---

## 19) Minimal QML/Bridge Integration Sketch

* `NodeModel` and `EdgeModel` expose instance data (pos/scale/color).
* QML `View3D` binds to these via `InstanceList`.
* `submitMessage()` triggers: tokenize → RAKE/SimHash → graph updates → layout relax → emit `rogerianReply`.

---

## 20) Licensing

* **Project:** Apache-2.0 or MIT.
* Ensure third-party headers (glm, ankerl, nlohmann, spdlog, tinyxml2, xxhash) are license-compatible and attributed in `/LICENSES`.