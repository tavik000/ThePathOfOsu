<p align="center">
  <h1 align="center">The Path of Osu (オスの道)</h1>
  <p align="center">UE5 Petit Con 21st — Unreal Engine 5 solo game project</p>
</p>

<p align="center">
  <a href="README.md"><b>English</b></a> | <a href="README.ja.md">日本語</a>
</p>

## Gameplay Video:
[![VIDEO](https://t3.ftcdn.net/jpg/04/03/98/64/360_F_403986499_hB7zfgOXezReA0sKkxl34RoT9TbNkbpH.jpg)](https://youtu.be/ikTBFtA-7Po)

## Screenshots

![image](./Screenshot/Osu01.png) <br>
![image](./Screenshot/Osu02.png) <br>
![image](./Screenshot/Osu03.png) <br>
![image](./Screenshot/Osu04.png) <br>
![image](./Screenshot/Osu05.png) <br>

### Keyword
> `UE` `Unreal Engine 5` `C++` `Puzzle Game` `Action Adventure` `TPS`



## Contents 
<!-- toc -->
* [Gameplay Video](#gameplay-video)
* [Screenshots](#screenshots)
* [Introduction](#introduction)
* [Environment](#environment)
* [Programming Notes](#programming-notes)
* [Author](#author)
* [License](#license)

<!-- toc stop -->


## Introduction

The Path of Osu is an Unreal Engine 5 game where you win by completing a variety of "Osu" missions, each one playing on a different meaning of the Japanese word 「押す (osu)」. It's a puzzle / action-adventure game.

This is my personal Unreal Engine 5 project. 80% C++, 20% Blueprint.

You can download the game from the [Releases](../../releases) page.

## Environment
- Unreal Engine 5
- Rider
- Windows

## Programming Notes

### Class Reference

#### `AOxCharacter`
Base class for all game characters. Handles general character logic — playing animations, attacking, movement, death, and HP.

#### `APlayerCharacter`
Player character class, inherits from `AOxCharacter`. Handles player input, third-person control, interacting with objects, the item inventory, item usage, and other player-specific logic.

#### `AEnemyCharacter`
Enemy character class, inherits from `AOxCharacter`. Handles the enemy HP bar and the lock-on UI logic; the enemy AI itself is implemented in Unreal Engine Blueprints.

#### `IInteractableInterface`
Interface for interactable objects — covers the highlighted-outline effect and requires implementers to define `Interact()`.

#### `APickup`
Actor class for pickup items, implements `IInteractableInterface`. The outline highlights when the player character is nearby, and pressing "E" picks the item up.

#### `APressableButton`
Button class that triggers an effect when pressed, implements `IInteractableInterface`. Usually linked to an `AMovableActor`, but it can be linked to other objects as well.

#### `UItem`
Defines the item data structure — item name, quantity, and so on.

#### `APushableActor`
A pushable block. Contains the logic that checks whether the player is pushing it, whether an obstacle blocks its path ahead, whether there's ground ahead, and other conditions that determine whether it can move.

#### `ATransporter`
Component that handles movement logic.

#### `AMovableActor`
Class for objects that can move, built around an `ATransporter` component. Defines where the object moves from and to.

#### `UAttackReflectableAnimNotify`
Anim-notify class marking the window within a character's attack animation during which the attack can be parried.

#### `UBlockAnimNotify`
Anim-notify class marking the active window within a character's block animation.

#### `UBlockReflectableAnimNotify`
Anim-notify class marking the parry window within a character's block animation.

#### `UFistAttackAnimNotify`
Anim-notify class marking the active window within a character's fist-attack animation.

#### `USlowMotionAnimNotifyState`
Anim-notify state marking the slow-motion window within a character's attack animation.

#### `UBlockMovementNotify`
Anim-notify class marking a window during which the character cannot move.

#### `UUseItemNotify`
Anim-notify class marking the exact moment within a character's item-use animation when the item takes effect.

#### `ACollectableActor`
An item that's automatically picked up when the player character touches it.

#### `AMainMenuPawn`
Handles the logic for displaying the main menu UI.

#### `UOsuMission`
Defines the "Osu" mission data structure — mission name, required count, and so on.

#### `AThePathOfOsuGameMode`
Base class for the game mode.

#### `ACompleteAllMissionGameMode`
Game-rules class. Shows the Game Over screen when the player dies, and the victory screen once all Osu missions are cleared.

#### `AMainMenuGameMode`
Game mode class required for Unreal Engine's main menu.

## Author
- <img src="https://github.com/favicon.ico" width="24">[Key](https://github.com/tavik000) <br>

-----


[⬆ Back to top](#contents)

**All Copyright Reserved**
