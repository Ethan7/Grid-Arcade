# Grid-Arcade
Grid-based Arcade platform using SDL2 and written entirely in C

## Screenshot
![Main Menu](https://raw.githubusercontent.com/Ethan7/Grid-Arcade/master/arcade-screen.png)

## Windows
Extract the game from the x86_64 zip file into one folder and play.

## Linux
Extract the game from the linux zip file into one folder, then give executable permissions to the folder with chmod and play.

## Compile on Linux
1) Install essential packages (for debian/apt users):
```
sudo apt-get install build-essential
sudo apt-get install gcc-mingw-w64
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
```

2) Follow the tutorials here in order to setup SDL2 source and SDL2-image source:
https://lazyfoo.net/tutorials/SDL/01_hello_SDL/linux/index.php
https://lazyfoo.net/tutorials/SDL/06_extension_libraries_and_loading_other_image_formats/linux/index.php

3) Clone this git repository in an adjacent folder to the SDL2 and SDL2-image source.

4) Enter main directiory and run make:
```
make
```

5) Done