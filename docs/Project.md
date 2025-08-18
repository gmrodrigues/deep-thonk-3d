Here’s a tight, product-facing overview you can drop into a README or a one-pager.

# deepThonk3d — Project Overview

## Elevator pitch

**deepThonk3d** is a local-first C++/Qt app that chats with you in a Rogerian (ELIZA-like) style while it **draws a live 3D mind map** of your thoughts. As you type, it extracts key ideas, links related concepts, and arranges everything in 3D using an incremental force-directed layout. No cloud, no setup.

## Who it’s for

* People who think best by talking/writing and want a **visual map** of their ideas.
* Therapists/coaches and reflective journaling users who value **non-directive prompts**.
* Researchers, writers, and founders exploring **themes, contradictions, and clusters**.

## Core use cases

* **Guided reflection:** gentle prompts → capture emotions, beliefs, and goals.
* **Idea cartography:** auto-generate nodes/links → discover structure and gaps.
* **Session replay:** scrub through time to see how themes emerge and connect.
* **Curation:** manually pin, relabel, or link nodes to clean the map.

## Key features (MVP → near-term)

* **Rogerian chat engine:** pattern/rule set for reflections and open questions.
* **Semantic extraction (on-device):** RAKE/TextRank-lite for keyphrases; SimHash/MinHash for quick similarity; co-occurrence scoring for edges.
* **3D visualization:** Qt Quick 3D with instancing for thousands of nodes/edges; incremental force layout (Barnes–Hut octree).
* **Editing tools:** select node, pin/unpin, merge, add manual links, tag, color by community.
* **Persistence & export:** local JSON (WASM) / optional SQLite FTS5 (desktop); export GraphML/GEXF for Gephi.
* **Privacy by design:** offline/local by default; no background network calls.

## Architecture (high level)

**Languages & runtime:** C++17+, Qt 6/QML, Qt Quick 3D; targets **desktop** (Win/macOS/Linux) and **WebAssembly** (Qt for WASM).

**Modules**

* `deepcore::graph_store` — nodes/edges, indices, serialization.
* `deepcore::nlp_light` — tokenization, stopwords (PT/EN), RAKE, SimHash.
* `deepcore::rogerian` — rules, reflection dictionary, response selection.
* `deepcore::layout3d` — force model, octree Barnes–Hut, neighborhood relaxation.
* `deepcore::storage` — JSON (all targets), SQLite/FTS5 (desktop only).
* `ui::bridge` — Qt/C++ ↔ QML (`QAbstractListModel`) for instances and commands.
* `ui::qml` — chat panel, 3D view, filters, time scrubber.

**Per-message data flow**

1. Normalize text → segment sentences.
2. Extract keyphrases + compute SimHash.
3. Create/update **Statement** and **Concept** nodes; add **Related/Mentions** edges.
4. Relax layout locally (e.g., 20–60 iterations in the affected subgraph).
5. Generate Rogerian reply; append to chat.

## Minimal data model

* `Node { id, kind: {Concept|Statement|Question|Tag}, text, weight, ts, hash }`
* `Edge { a, b, kind: {Related|Mentions|Supports|Contradicts|Causes}, weight }`
* Indices: by `id`, by `hash` bucket, by time, optional full-text (desktop).

## UX outline

* **Left:** chat history + input (Enter to send), quick actions (Pin, Tag, Merge).
* **Right:** 3D map — orbit/zoom, hover details, click to focus, neighborhood highlight.
* **Top bar:** search, filters (time, tag, community), screenshot/export.
* **Timeline:** drag to replay how the map formed.

## Non-functional targets (initial)

* **Performance:** smooth interaction up to \~3–5k nodes / \~8–12k edges on desktop; \~1–2k nodes in WASM.
* **Frame rate:** target ≥ 45 FPS desktop, ≥ 30 FPS WASM during layout relaxation.
* **Start time:** < 2s desktop, < 5s WASM (first load).
* **Memory cap (WASM):** keep under \~256–512 MB total.

## Build & packaging

* **CMake presets:** `desktop-debug`, `desktop-release`, `wasm-release`.
* **Flags:** `-DWEB=ON`, `-DENABLE_SQLITE=ON` (desktop only), `-DELIZA_ONLY` (minimal), `-DNLP_ENHANCED` (RAKE/SimHash).
* **Output:** single desktop binaries; WASM bundle (HTML + JS glue).

## Roadmap

**v0.1 (MVP)**
Chat + capture → nodes/edges + incremental layout; JSON save/load; GraphML export; basic color by degree/community.

**v0.2 (Alpha)**
Pin/merge, tags & filters, neighborhood relax tuning, screenshots, keyboard shortcuts.

**v0.3 (Beta)**
Time scrubber, clustering improvements, edge bundling light, theming, SQLite FTS5 on desktop.

**v0.4+ (Optional add-ons)**
Local LLM plugin (desktop) via `llama.cpp`; offline STT/TTS; plugin API for custom extractors or layouts.

## Risks & mitigations

* **Graph sprawl / visual clutter:** progressive disclosure, pinning, clustering, edge fade/bundling.
* **WASM limits:** avoid SQLite; reduce geometry (line lists over cylinders); cap active relax iterations.
* **Multilingual text:** separate stopword lists; fallback to n-grams when unknown.
* **Therapeutic claims:** clear “not clinical advice” disclaimer.

## Success metrics

* Creation: nodes/session, edges/node, sessions/week.
* Comprehension: time-to-insight (self-reported), % of sessions with manual curation (pin/tag).
* Retention: D7/D30 return rate; export usage.

## Licensing & privacy

* Recommend **Apache-2.0** (or MIT).
* Local-first; explicit opt-in for any telemetry; readable JSON export for portability.

## Open questions

* Do we want **audio** (STT/TTS) in the MVP or as a plugin only?
* Should “emotional tone” (simple sentiment) affect layout/color?
* Where to draw the line between **automatic** versus **manual** linking?
