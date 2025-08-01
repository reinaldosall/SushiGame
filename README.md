# SushiGame

# 🍣 Sushi Game

**Sushi Game** is a multiplayer cooking prototype developed in **Unreal Engine 5.6**, with all core systems implemented in **C++**. Players collaborate in a shared kitchen to prepare and deliver sushi dishes under time pressure.

---

## 🎮 Gameplay Summary

- Host or join a multiplayer session (listen server)
- Prepare sushi by slicing, rolling, and cooking ingredients
- Deliver dishes to the correct table before the timer expires
- Earn points for correct deliveries, lose points for mistakes or late orders
- Match ends in Victory or Defeat based on the global score

---

## 🧱 Architecture

- **Match flow:** handled with a replicated `MatchState` enum (Lobby → InGame → Victory/Defeat)
- **Actors:** modular `CookwareActor`, `IngredientActor`, `TableActor`
- **UI:** fully in C++ using `UUserWidget` (Lobby, HUD, Pause Menu, End Game)
- **Replication:** stateful logic for orders, score, progress, and HUD sync
- **Maps:** `Lvl_Lobby` (host/join UI) and `Lvl_Restaurant` (core gameplay)

---

## 🖥️ Controls

| Key     | Action                   |
|---------|--------------------------|
| WASD    | Move                     |
| Mouse   | Look around              |
| **E**   | Interact (e.g. cookware) |
| **F**   | Deliver dish             |
| **TAB** | Open pause menu          |

---

## 📦 Build & Run Instructions

1. Download the ZIP file from the build link (see below)
2. Extract it and run `SushiGame.exe` inside `WindowsNoEditor/`
3. To host: press **Host Game**
4. To join: press **Join Game** (on another instance of the game)

---

## 📝 Project Files

- `Project_Description.pdf`: development summary
- `SushiGame.uproject`: source project (in the repository)
- `WBP_*`: UMG-based UI widgets
- All gameplay logic implemented in C++

---

## 📥 Download Build

👉 [Link to Windows Build (Google Drive)](https://drive.google.com/drive/folders/1fx-bVTOgTW3iRGHyiZJLMnZuoH4pEjNs?usp=sharing)

---

## ⏱️ Total Development Time

**~42 hours total** (spread across 8 days of part-time effort)

### Time Breakdown by Phase

| Phase                                      | Time |
|--------------------------------------------|------|
| 1. Project Setup & Core C++ Architecture   | 6h   |
| 2. Multiplayer Logic & Replication (C++)   | 10h  |
| 3. Gameplay: Orders, Cooking, Delivery     | 10h  |
| 4. UI/UX Design (Lobby, HUD, End Screens)  | 6h   |
| 5. Polish, Bugfixing & Build Packaging     | 6h   |
| 6. Readme, Documentation & Submission Prep | 4h   |

> All logic and UI flow were implemented in C++ with replication in mind. Time was distributed to prioritize clarity, stability, and a complete game loop.

---

## ✅ Final Thoughts

Everything was designed to be clean, responsive, and robust under multiplayer conditions. I enjoyed the challenge and applied best practices to keep the code readable, modular, and game-ready.

---

## 📃 License & Credit

- 3D assets from **Quaternius Sushi Restaurant Kit**  
- Developed as part of the **NGP Unreal Developer Task**  
- All code, gameplay logic, UI flow, and networking created by me.

Made with care by **Reinaldo Saldanha**
