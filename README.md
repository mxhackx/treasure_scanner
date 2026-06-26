# 🔍 Treasure Scan

A real-time SFML grid-based treasure discovery simulation with procedural generation and animated reveal effects.

---

## 🎮 What is this?

Treasure Scan is a small graphical simulation where a hidden treasure is placed inside a grid.  
The grid is gradually revealed cell by cell… until the treasure is found.

Once discovered, the game triggers a full animation sequence:
- zoom effect 📦
- movement transition 🧭
- state-based UI reveal 📊

---

## ✨ Features

- 🧱 Procedural grid generation
- 👀 Progressive cell reveal system
- 💎 Randomized tile system (Gem / Cross / Ring / Treasure)
- 🎬 Multi-stage treasure animation
- ⏱️ Time tracking system
- 📊 Live counters (cells revealed + time elapsed)
- 🎨 SFML real-time rendering

---

## 🚀 How it works

1. A grid is generated randomly
2. One hidden treasure is placed
3. Cells are revealed over time
4. When the treasure is found:
   - animation starts
   - camera-like movement effect triggers
   - UI displays results

---

## 🧠 Concepts used

- State machine logic (state 0 → 1 → 2)
- Time-based animation
- Procedural generation
- Sprite management
- Basic game loop architecture

---

## 🖥️ Build & Run

### Requirements
- SFML 2.x
- C++17 or higher

### Compile
```bash
g++ main.cpp -o treasure_scan -lsfml-graphics -lsfml-window -lsfml-system
```
It looks simple now, but it took me 2 days to learn C++ and SFML 😢. Let’s continue 😁 — next step: Guess the Hidden Number.
