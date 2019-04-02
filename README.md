# M5Apple2
M5Apple2 is an Apple ][ emulator for M5Stack microcontroller, based on LinApple (http://linapple.sourceforge.net/).

## Prerequisites for Play
* M5Stack FIRE (https://docs.m5stack.com/#/en/core/fire)
* Micro SD Card (TF Card)
* Apple ][ Disk Image
* (Recommended) M5Stack_LovyanLauncher (https://github.com/lovyan03/M5Stack_LovyanLauncher)
* (Optional) Grove Thumb Joystick (Analog Joystick)
* (Optional) M5Stack Official CardKB Mini Keyboard.

## Preparation for Play
* Build and upload M5Apple2, or install LovyanLauncher and copy a program in "bin" folder.
* Copy some Apple ][ Disk Image(s) to "/m5apple2" folder on Micro SD Card.
* Insert the Micro SD Card to M5Stack.

## Play
1. Turn m5stack power on then automatically start.
1. Push Button-A for Menu.
1. Select "Insert Disk #1" and Select a Disk Image.
1. Select "Reset" for booting Apple ][ emulator.

## Keyboard
* Support for M5Stack OnScreenKeyboard (soft keyboard).
* Support for CardKB Mini Keyboard.

## Joystick
* Support for on-board Accelerometer Joystick Emulation.
* Support for Grove Thumb Joystick.
* Use Button-B and Button-C on M5Stack for Joystick buttons.

### Accelerometer Joystick Calibration
1. Place M5Stack in a horizontal position.
1. Push Button-A for Menu.
1. Select "Select Joystick: Accelerometer".
1. Select "Calibrate Joystick". (Don't move m5stack)
1. Wait until dialog box closed.

### Analog Joystick Calibration
1. Place M5Stack in a horizontal position.
1. Push Button-A for Menu.
1. Select "Select Joystick: Analog".
1. Select "Calibrate Joystick". (Don't move thumb stick)
1. After "Move stick vertically and holizontally." message, move thumb stick vertically and holizontally several times.

## SD-Updater
* M5Apple2 uses SD-Updater. Booting M5Stack with Button-A pressed will start SD-Updater.

# Unsupported functions
* Sounds
* Saving files
* Serial Communication
* Printers

