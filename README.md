# Car Game

An OpenGL-based scene renderer and driving mini-game, built as an extension of **Comp3015-30**. It implements advanced rendering techniques including shadow mapping, particle effects, and noise mapping, as well as a checkpoint-based driving challenge.

**Development environment:**
- **IDE:** Visual Studio 2022
- **OS:** Windows 10
- **GPU:** AMD

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
  - [Rendering Enhancements](#rendering-enhancements)
  - [Gameplay Elements](#gameplay-elements)
- [Architecture & Key Classes](#architecture--key-classes)
  - [Shader System](#shader-system)
  - [SceneSwitcher](#sceneswitcher)
  - [Input Handler](#input-handler)
  - [SaveDataManager](#savedatamanager)
  - [MapLoader](#maploader)
  - [CollisionManager](#collisionmanager)
  - [ImGui Integration](#imgui-integration)
- [Controls](#controls)
  - [Start Scene](#start-scene)
  - [Game Scene](#game-scene)
- [Build & Run](#build--run)
- [Video Demo](#video-demo)

---

## Overview

This project extends the original Comp3015-30 coursework by introducing:

- **Shadow Mapping:** Spotlights cast realistic, dynamic shadows.
- **Particle Effects:** GPU-accelerated particle systems.
- **Noise Mapping:** Used for procedural clouds and environment effects.
- **Customisable Car:** Select car textures in the start scene and use them in the game scene.
- **Driving Mini-Game:** Complete a checkpoint-based race while avoiding obstacles and beating your best time.

---

## Features

### Rendering Enhancements

- **Shadow Mapping:** Each spotlight uses shadow buffers for added depth and realism.
- **Particle Effects:** Emitters create dynamic effects with physics-based movement.
- **Noise Mapping:** Procedural shaders produce evolving clouds and surface textures.

### Gameplay Elements

- **Car Customisation:** Choose from seven pre-defined textures with optional layered bindings.
- **Persistent Save System:** Retains best time and chosen car texture between runs.
- **Interactive Game Loop:** A mini-racing game with visual checkpoint guidance and physical obstacles.

---

## Architecture & Key Classes

### Shader System

Custom include-based system to modularise shader code. Uses a global settings UBO to store lighting and shadow configurations accessible to all shaders.

### SceneSwitcher

A singleton that handles transitions between scenes.

**Features:**
- Deferred switching  
- Initialisation tracking  
- Singleton access

### Input Handler

A singleton keyboard manager tracking both current and previous key states.

**Features:**
- isKeyPressed(key) – returns true while held  
- isKeyPressedOnce(key) – returns true only when the key is pressed for the first time  
- updateKeyState() – updates states each frame

### SaveDataManager

Singleton that manages save data (car texture and best time) using nlohmann::json and std::filesystem.

**Features:**
- Serialises to data/save_data.json
- Converts to/from JSON automatically
- Persistent across game runs

### MapLoader

Reads a JSON-formatted level file and instantiates game objects using a factory system.

**Features:**
- Enables easy map creation through easy to understand json.
- Type registration via register_type<T>("TypeName") 
- Auto placement in world space
- Multi-interface support for scene, particle, collision, light, uploader, and checkpoint management

### CollisionManager

Manages detection and notification of collision events.

**Features:**
- Runtime registration of colliders
- Continuous detection (detectAndNotify)
- One-time detection (detectOnceAndNotify)

---

### ImGui Integration

The project integrates **Dear ImGui** to support developer and player-facing UI menus in both scenes.

#### Features:

- **Start Scene Menu:**
  - Appears as a right-hand panel on screen.
  - Lets the player begin the game.
  - Players can see their current best time for the driving minigame above the play button.

- **Game Scene Pause Menu:**
  - Toggle with the `P` key.
  - Displays the current elapsed time in the driving challenge.
  - Provides a "Return to Menu" button that returns the player to the start scene and preserves their car selection.

## Controls

### Start Scene

#### Car Rotation
- Space – Toggle automatic rotation  
- Left / Right Arrow – Manual rotation / speed change

#### Texture Selection
The car’s texture can be changed by pressing the following number keys:
- **Key 1:** Use **orange** texture.
- **Key 2:** Use **black** texture.
- **Key 3:** Use **blue** texture.
- **Key 4:** Use **dark blue** texture.
- **Key 5:** Use **dark grey** texture.
- **Key 6:** Use **grey** texture.
- **Key 7:** Use **red** texture.

*Note: Holding **Left Shift** while selecting a texture will selectively bind only one texture layer. This allows user to make custom colours based on the combinations they use*

#### Camera
- **Left Mouse Button:** Engage camera rotation mode (cursor is hidden during dragging).
- **Mouse Drag:** Rotate the camera. Only works if holding left mouse button
- **Scroll Wheel:** Zoom in and out.
---

### Game Scene

#### Core Game Loop

In the game scene, your objective is to **drive through all checkpoints in sequence** while avoiding obstacles (barrels) on the map.

- **Goal:** Reach each checkpoint in order to complete the course.
- **Guidance:** The **active checkpoint** is lit with a **spotlight**, helping guide you to your next destination.
- **Obstacles:** Barrels are placed throughout the map. Colliding with them will slow or stop your car.
- **Completion:** Finish all checkpoints as quickly as possible to set a best time.
- **Progress Tracking:** Your current time is displayed in the pause menu (P), and your best time is saved between runs.

---
#### Driving
- W – Accelerate  
- S – Brake / reverse  
- A / D – Steer left/right (turn strength scales with speed)

#### Driving Physics
- **Drift:** Automatic sideways slip when turning
- **Body Roll:** Car leans into turns dynamically

#### Camera
- Automatically follows the player’s car

---

## Build & Run

- Clone and run the repository anywhere. Make sure the neccesary include and lib files are present at C:\Users\Public\OpenGL

---

## Video link

