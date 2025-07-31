# SushiGame

# 🍣 Sushi Game

**Sushi Game** is a multiplayer cooking prototype developed in Unreal Engine 5.6 using C++ for all core systems. Players collaborate in a shared kitchen to prepare and deliver sushi dishes under time pressure.

## 🎮 Gameplay Summary

- Host or join a multiplayer session (listen server)
- Prepare sushi by slicing, rolling, and cooking ingredients
- Deliver dishes to the correct table before the timer expires
- Earn points for correct deliveries, lose points for mistakes
- Match ends in Victory or Defeat based on total score

## 🧱 Architecture

- **Match flow:** handled with a replicated `MatchState` enum (Lobby → InGame → Victory/Defeat)
- **Actors:** modular `CookwareActor`, `IngredientActor`, `TableActor`
- **UI:** fully in C++ using `UUserWidget` (Lobby, HUD, Pause Menu, End Game)
- **Replication:** stateful logic for orders, score, progress, and HUD sync
- **Maps:** `Lvl_Lobby` (host/join UI) and `Lvl_Restaurant` (gameplay)

## 🖥️ Controls

- **WASD / Mouse:** Move and look around
- **E:** Interact with cookware
- **F:** Deliver dish
- **TAB / ESC:** Open pause menu

## 📦 Build & Run Instructions

1. Download the ZIP file from the build link (see below)
2. Extract it and run `SushiGame.exe` inside `WindowsNoEditor/`
3. To host: press **Host Game**
4. To join: press **Join Game** (on another instance of the game)

## 📝 Files

- `Project_Description.pdf`: development summary
- `SushiGame.uproject`: source project (in code branch)
- `WBP_*`: UI elements created in UMG
- All gameplay logic is written in C++

## 📥 Download Build

👉 [Link to Windows Build (Google Drive/OneDrive)](https://drive.google.com/drive/folders/1fx-bVTOgTW3iRGHyiZJLMnZuoH4pEjNs?usp=sharing)

## 📃 License & Credit

Assets from the **Quaternius Sushi Restaurant Kit**  
Project developed as part of digital games engineer selection process test attendee **NGP Unreal Developer Task**

---

Made with care by **Reinaldo Saldanha**
