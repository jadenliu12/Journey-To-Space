# Journey-To-Space

## Basic Informations  
Description: A simple sky fighter game where the goal is to defeat the boss at the end  
Language: C with allegro  
Features:  
- Different choices of plane for the user, each with unique abilities  
- Purchasable upgrades for the user's plane each time they have cleared a stage  
- High-score tracker  
- Sound effects (shooting SFX, purchasing SFX, etc)  
- Ultimate attack  
- User's plane info (current health, accumulated coins, and current score)  

## Allegro set-up
IDE: Code::Blocks  
Download link: [allegro-mingw-gcc6.2.0-x86-static-5.2.2.zip](https://github.com/liballeg/allegro5/releases/download/5.2.2.0/allegro-mingw-gcc6.2.0-x86-static-5.2.2.zip)  
  
After finish downloading, Un-zip the file and move it to your C drive. The full path should be like: `C:\allegro-mingw-gcc6.2.0-x86-static-5.2.2\allegro`. After that see the [Allegro codeblocks settings](/Allegro%20codeblocks%20settings.txt) and modify the project's build settings so that the allegro library can be used.  

## Code flow
![Code flow chart](/Images/CodeFlow.jpg)  

## Game informations  
### Goal  
Defeat the boss and unlock all the planes by getting the highest highscore!  
### Specifications  
User's plane specifications:  
- HP (original) = 15  
- Speed (original) = 4  
- Bullet damage = 1
- Bullet speed (original) = 1  
- Bullet per fire (original) = 1  
- Maximum bullet = 4  
User's upgrades:  
- Health upgrade = increase current health by 1  
- Bullet speed upgrade = increase current speed by 3  
- Ultimate upgrade = allows the user to use the plane's ultimate attack  
Enemy's plane specifications:
- HP = 1  
- Speed = 1 and at each stage will be increased by 1  
- Bullet damage = 1  
- Bullet speed = 3 and at each stage will increased by 2  
- Maximum enemy bullet = 8  
- Maximum enemy = 5  
Boss's minion specifications:  
- HP = 1  
- Speed = 3 and each minion moves in a different direction  
- Maximum minion = 4  
Boss's specifications:  
- HP = 100  
- Speed = 5  
- Laser attack damage = 3  
### Cheats  
From the menu scene, the user can press these button to cheat:  
- '1' = winning scene  
- '2' = losing scene  
- '3' = gain 1000 money and skip stage 1  
- '4' = gain 1000 money and skip stage 2  
- '5' = gain 10000 money and skip stage 3  

## In-game photos  
### Menu scene  
![Menu scene](/Images/menu.jpg)  
### Settings scene  
![Settings scene](/Images/settings.jpg)  
### Stage 1 scene  
![Stage 1 scene](/Images/start.jpg)  
### Stage 2 scene  
![Stage 2 scene](/Images/stage2.jpg)  
### Stage 3 scene  
![Stage 3 scene](/Images/stage3.jpg)  
### Boss scene  
![Boss scene1](/Images/boss.jpg)  
![Boss scene2](/Images/bossAtk.jpg)  
![Boss scene3](/Images/ultimate.jpg)  
### Winning scene  
![Winning scene](/Images/win.jpg)  
### Losing scene  
![Losing scene](/Images/lose.jpg)  
### Transition scene  
![Transition scene](/Images/transition12.jpg)  
