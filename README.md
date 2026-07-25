<p align="center">
  <h1 align="center">オスの道　(The Path of Osu)</h1>
  <p align="center">ぷちこん21th　Unreal Engine 5　個人ゲーム作品</p>
</p> 



### Keyword
> `UE` `Unreal Engine 5` `C++` `Puzzle Game` `Action Adventure` `TPS`



## Contents 
<!-- toc -->
* [Introduction](#introduction)
* [ゲーム紹介](#ゲーム紹介)
* [Environment](#environment)
* [プログラム解説](#プログラム解説)
* [Author](#author)
* [Gameplay Video](#gameplay-video)
* [Screenshots](#screenshots)
* [License](#license)

<!-- toc stop -->


## Introduction

This is my Unreal Engine 5 Project.

80% C++, 20% Blueprint.

Download in Release;
リーリスでゲームをダウンロードできますよ

## ゲーム紹介

オスの道　はUnreal Engine 5 を使って色んな「オス」任務を完成したら勝ちというゲームです。パズル、アクションアドベンチャーゲームです。

## Environment
- Unreal Engine 5
- Rider
- Windows

## プログラム解説

### Class 解説

#### `AOxCharacter`
ゲームキャラクターのBase Classです、アニメーションをプレイ、攻撃、移動、死亡、Hpなどキャラクター全般のロジック。

#### `APlayerCharacter`
プレイヤーのキャラクタークラス、AOxCharacterを継承した。プレイヤーインプット、第三人稱操作。物をinteractするロジック、アイテムインベントリー、アイテムの使用などプレイヤーを関係するロジック全般。

#### `AEnemyCharacter`
敵キャラクタークラスです、AOxCharacterを継承した。敵HP bar、敵をロックオンのUIロジック、敵AIを実装する部分はUnreal Engine のBlueprintにあります。

#### `IInteractableInterface`
操作できる物のインタフェースクラス、アウトラインが光るや、Interact()が必ず実装するという意味も含めます。

#### `APickup`
拾うアイテムのUnreal Engine Actorクラスです。IInteractableInterfaceを実装した。プレイヤーキャラクターが近くとアウトラインが光る、キーボード「E」を押すと拾います。

#### `APressableButton`
押すと何が起きるバタンクラス、IInteractableInterfaceを実装した。AMovableActortと一緒に連動するのは一般ですが、他のものと連動することもできます。

#### `UItem`
アイテムデータのStructureをきめるクラスです、アイテム名前、数量など。

#### `APushableActor`
押すと移動するもの、ブロック。プレイヤーが押していますがのを確認するや、この先は邪魔ものがあるかどうか、この先は地面があるかどうか、色んな移動できる状況を確認するロジックがあります。

#### `ATransporter`
移動ロジックのComponentです。

#### `AMovableActor`
移動できる物のクラスです、ATransporterのcomponentがあります。どこからどこまで移動するのがここに決める。

#### `UAttackReflectableAnimNotify`
アニメーションのタイミングを関するクラス。
キャラクターの攻撃アニメーションの中にパリーできる間のタイミングのフラグを示します。

#### `UBlockAnimNotify`
アニメーションのタイミングを関するクラス。キャラクターのブロックアニメーションの中に有効の間のタイミングを示します。

#### `UBlockReflectableAnimNotify`
アニメーションのタイミングを関するクラス。キャラクターのブロックアニメーションの中にパリーできるの間のタイミングを示します。

#### `UFistAttackAnimNotify`
アニメーションのタイミングを関するクラス。キャラクターのアタックアニメーションの中に有効の間のタイミングを示します。

#### `USlowMotionAnimNotifyState`
アニメーションのタイミングを関するクラス。キャラクターのアタックアニメーションの中にスローモーションの間のタイミングを示します。

#### `UBlockMovementNotify`
アニメーションのタイミングを関するクラス。キャラクターがこの間に移動出来ません。

#### `UUseItemNotify`
アニメーションのタイミングを関するクラス。キャラクターの道具を使用するアニメーションの中に有効の瞬間タイミングを示します。

#### `ACollectableActor`
プレイヤーキャラクターが触ると自動的拾うアイテムです

#### `AMainMenuPawn`
Main Menu UIを出せるのロジックがあります

#### `UOsuMission`
オス任務データのStructureをきめるクラスです、ミッションの名前、回数など。

#### `AThePathOfOsuGameMode`
ゲームモードのBase Class

#### `ACompleteAllMissionGameMode`
ゲームルールのクラスです、プレイヤーが死亡したらGame Overスクリームを出せる。オス任務全部クリアしたら勝ちスクリームを出せる。

#### `AMainMenuGameMode`
Unreal Engine メインメニューが必要になるゲームモードクラスです、

## Author
- <img src="https://github.com/favicon.ico" width="24">[Key](https://github.com/tavik000) <br>


## Gameplay Video:
[![VIDEO](https://t3.ftcdn.net/jpg/04/03/98/64/360_F_403986499_hB7zfgOXezReA0sKkxl34RoT9TbNkbpH.jpg)](https://youtu.be/ikTBFtA-7Po)

## Screenshots

![image](./Screenshot/Osu01.png) <br>
![image](./Screenshot/Osu02.png) <br>
![image](./Screenshot/Osu03.png) <br>
![image](./Screenshot/Osu04.png) <br>
![image](./Screenshot/Osu05.png) <br>

-----


[⬆ Back to top](#contents)

**All Copyright Reserved**
