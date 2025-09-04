# Data Structures & Algorithms (2024–2025)
Build core data structures and apply them to neural networks across a three‑part coursework series.

## Overview
This repository is a single evolving codebase for three graded assignments:
- **Assignment-1:** Lists + dataset utilities + MLP **inference**.
- **Assignment-2:** HashMap & Heap + MLP **training** (extends Assignment-1).
- **Assignment-3:** Graph API + **topological order** for ANN compute graphs (extends Assignment-1/2).

> Design intent: keep one cohesive repo so later parts reuse and extend earlier abstractions.

---

## Prerequisites
- **C++17** toolchain (tested with `g++`).
- Linux/macOS/WSL or Git Bash/MinGW on Windows.
- Bundled third‑party headers: **xtensor** (tensors), **fmt** (formatting).

---

## Repository Structure
```
.
├─ include/                         # Public headers
│  ├─ list/                         # XArrayList, DLinkedList (Assignment-1)
│  ├─ loader/                       # Dataset, TensorDataset, DataLoader (Assignment-1)
│  ├─ hash/                         # HashMap (Assignment-2)
│  ├─ heap/                         # Heap (Assignment-2)
│  ├─ ann/                          # ANN components (inference & training)
│  ├─ tensor/                       # xtensor wrappers
│  ├─ sformat/                      # fmt helpers
│  ├─ graph/                        # IGraph, AbstractGraph, DGraphModel, UGraphModel (Assignment-3)
│  └─ util/, dsaheader.h
├─ src/                             # Implementations (ann/, tensor/, program.cpp, …)
├─ demo/                            # How-to snippets for DS/ANN
├─ datasets/                        # Training/eval data (user-provided)
├─ models/                          # Saved model artifacts
├─ config.txt                       # Runtime configs (paths, hyperparams, etc.)
├─ Makefile
└─ compilation-command.sh
```

---

## Quick Start
**Option A — Script**
```bash
./compilation-command.sh
```

**Option B — Makefile**
```bash
make                # build
make clean && make
```

**Option C — IDE/Manual**
```bash
g++ -std=c++17 -Iinclude -Isrc   src/program.cpp   -o build/app
```

Run your binary (path may vary):
```bash
./build/app
```

---

## Details

### Assignment-1 — Lists + Datasets + Inference
**Deliverables**
- `XArrayList` (array-backed) and `DLinkedList` (doubly-linked).
- Dataset utilities using **xtensor** for N-D arrays.
- Minimal MLP **inference** pipeline (forward pass; no training).

**Development tips**
- Ship the **list** module first (compile & test in isolation).
- Add `Dataset` / `TensorDataset` + `DataLoader` (support `foreach` over batches).
- Wire up the MLP inference components under `include/ann/` and `src/ann/`.

---

### Assignment-2 — HashMap + Heap + Training
**Deliverables**
- `HashMap` in `include/hash/xMap.h`  
  - Separate chaining with `DLinkedList` buckets.  
  - Load‑factor tracking & **rehash** policy.
- `Heap` in `include/heap/Heap.h` (generic comparator/arity if required).
- Extend the ANN to support **training** (multi-layer feed‑forward).

**Critical requirement**
- Introduce a **backward iterator** for `DLinkedList` supporting `!=`, backward `++`, and `*`.

**Recommended workflow**
1. Temporarily exclude ANN from the build for faster iteration.  
2. Implement & test `HashMap` and `Heap` via `demo/hash` and `demo/heap`.  
3. Re‑enable ANN and plug training logic.

---

### Assignment-3 — Graphs + Computational Graph (Topological Sort)
**Deliverables**
- Graph stack (adjacency‑list based):
  - `IGraph<T>` — pure interface.
  - `AbstractGraph<T>` — partial impl, iterators & helpers.
  - `DGraphModel<T>` (directed) and `UGraphModel<T>` (undirected).
- **Topological sort** to schedule forward/backward passes on the ANN compute **DAG**.

**Goal**
- Use topo order to enforce dependency‑correct execution in forward/backprop.

---

## Data, Models, Config
- Put raw data under `datasets/`.
- Persist trained weights under `models/`.
- Tune paths/hyperparameters in `config.txt`.

---

## Build Notes & Troubleshooting
- Always compile with **`-std=c++17`**.
- If headers aren’t found, verify include paths: `-Iinclude -Isrc`.
- On Windows/MinGW, avoid spaces in build output paths or quote them properly.
- If you see linker issues, ensure all required `src/*` implementations are included in your build command or Makefile targets.

---

## Contribution Guide (coursework context)
- Keep public headers in `include/...`, implementations in `src/...`.
- Prefer templates/iterators and value semantics where possible.
- Document complexity (Big‑O) for each public method in comments.
- Add minimal demos in `demo/` when introducing a new data structure.

---
