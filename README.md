# Helldivers-2-Internal-Hack-Dll-Proxy-PoC

#### This is PoC that i create to learn about dll proxy using C++ on 64bit application / game.

### Thanks to cfemen and gir489 for all information and CE Tables.

## Feature is :
```c
//Console Menu
std::vector<Checkbox> checkboxes = { 
      {"Inf Health", false}
    , {"Inf Grenades", false}
    , {"Inf Grenades(Legit)", false}
    , {"Inf Ammo", false}
    , {"Inf Ammo(Legit)", false}
    , {"Inf Syringes", false}
    , {"Inf Syringes(Legit)", false}
    , {"Inf Stamina", false}
    , {"Inf Stratagems", false}
    , {"Inf Mission Time", false}
    , {"No Reload", false}
    , {"Max Resources", false}
    , {"Add 5 Samples", false}
    , {"No Recoil", false}
    , {"Inf Backpack", false}
    , {"Inf Special Weapon", false}
    , {"No Laser Cannon Overheat", false}
    , {"Instant Railgun", false}
    , {"Show All Map Icons", false}
    , {"No Stationary Turret Overheat", false}
    , {"No Backpack Shield Cooldown", false}
    , {"No JetPack Cooldown", false}
    , {"All Stratagems in Loadout", false}
    , {"All Equipment in Armory", false}
    , {"All Armor in Armory", false}

};
```

## How to use :
### - Download DLL
### - Extract DLL inside helldivers 2 game folder ( same folder as helldivers2.exe ), and rename it to version.dll
### - Run Game
### - Choose Feature ( Navigate using arrow key to up/down, space for select / unselect )
### - Enter To Applied Feature
### - Happy Cheating

### Doesnt need old exe, you can use latest exe.

# Building 
You don't need to build this in Visual Studio, the most recent versions are posted here:\ https://github.com/emoisback/Helldivers-2-Internal-Hack-Dll-Proxy-PoC/actions \
Because github automatically builds the master branch, you can:
- Fork the repository. 
- Go to the Actions tab and make sure workflow is enabled.
- Make any changes to a file, and github will rebuild the .dll in the Actions tab of that fork.

For example: https://github.com/DeathWrench/Helldivers-2-Internal-Hack-Dll-Proxy-PoC/actions \
As you can see, commits from forks get built by github as long as they're in the master branch.
#### Still if you want to build it yourself in Visual Studio:
![image](https://github.com/DeathWrench/Helldivers-2-Internal-Hack-Dll-Proxy-PoC/assets/45341450/cd8bb59e-72fb-492e-be0d-1a952295e27c)\
![image](https://github.com/DeathWrench/Helldivers-2-Internal-Hack-Dll-Proxy-PoC/assets/45341450/d7ef335a-ff96-48d0-bce6-e6bf2445f264)\
File will be here: \
``Helldivers-2-Internal-Hack-Dll-Proxy-PoC\x64\Release\``**version.dll**
