# M5Apple2
M5Apple2 is an Apple ][ emulator for M5Stack microcontroller, based on LinApple (http://linapple.sourceforge.net/).

## Prerequisites for Build
* Visual Studio Code (https://code.visualstudio.com/)
* Platform IO (vscode extension)
* M5Stack Library
* M5Stack SD-Updater (https://github.com/tobozo/M5Stack-SD-Updater)
* M5Stack OnScreenKeyboard (https://github.com/lovyan03/M5Stack_OnScreenKeyboard)
* MPU6050_tockn (https://github.com/Tockn/MPU6050_tockn)

## Preparation for Build
* Install Visual Studio Code.
* Install PlatformIO IDE for Visual Studio Code. (show https://docs.platformio.org/en/latest/ide/vscode.html)
* At "Libraries" tab of "PIO Home" page, install "M5Stack SD-Updater", "M5Stack OnScreenKeyboard", "MPU6050_tockn".
* Clone this git repository to a suitable folder.

`git clone https://github.com/EiichiroIto/m5apple2.git`

## Build
1. Click "New Project" at "Home" tab of "PIO Home" page.
1. Enter "m5apple2" to the "Name" field.
1. Select "M5Stack FIRE" of the "Board" field.
1. Select "Arduino" of the "Framework" field.
1. Check "Location" box off and open the folder including above repository. (not the repository folder, but the parent folder)
1. Click "Finish" button.
1. Delete "main.cpp" from "src" folder on a M5STACK workspace of the explorer.
1. From "Terminal" menu, select "Run a task", then select "PlatformIO:Build" to build, or "PlatformIO:Upload" to upload.
