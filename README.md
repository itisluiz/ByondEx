# What is Byond
Byond is a game engine that allows you to join user created games which can vary a lot in what they are but work in the same engine.

- byond.exe: This is the launcher, where you login and browse games
- dreamseeker.exe: This is the game client, where the game runs when you join a server, byond calls them "worlds"
- dreammaker.exe: This one compiles games made by users like this one [Paradise Station (ss13)](https://github.com/ParadiseSS13/Paradise.git)
- dreamdaemon.exe: This hosts the games combiled by dreammaker.exe, used to host your own worlds.

# What is this project
It was supposed to be a simple cheat menu, which I even intended to embed lua on, but I'm hitting wall after wall, turns out the game is full of anti-debugging techniques!

# What the current issue is
Apparently, when setting a [wndproc hook](https://github.com/itisluiz/ByondEx/blob/main/src/hooks/windows.cc#L91) some wacky behaviour starts to appear, like how allocated consoles
will start getting allocated in all byond processes, some other quirky stuff is going on with `SetWindowsHookEx`, like how if you don't set the wndproc hook and set a keyboard hook
the keyboard io will break for the entire computer (????).

# How to test this
1. Open byond, register or use the following account `Kyat:Correberg123`
2. You can either run your own server [Paradise server setup](https://github.com/ParadiseSS13/Paradise/blob/master/.github/DOWNLOADING.md) (Can be a bit of a hassle, I could try sending the compiled world for running in dreamdaemon.exe), or join a public server from the browser.
3. Inject byondex.dll in dreamseeker.exe


# TL; DR
Something windows input weird ugh
