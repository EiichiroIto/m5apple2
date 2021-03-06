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
1. Push Button-B for Menu.
1. Select "Insert Disk #1" and Select a Disk Image.
1. Select "Reset" for booting Apple ][ emulator.

## Keyboard
* Support for M5Stack OnScreenKeyboard (soft keyboard).
* Support for CardKB Mini Keyboard.

## Joystick
* Support for on-board Accelerometer Joystick Emulation.
* Support for Grove Thumb Joystick.
* Support for M5Stack I2C Joystick.
* Use Button-A and Button-C on M5Stack for Joystick buttons.

### Accelerometer Joystick Selection & Calibration
1. Place M5Stack face's up on a horizontal position like a table.
1. Push Button-B for Menu.
1. Select "Select Joystick: Accelerometer".
1. Select "Calibrate Joystick". (Don't move m5stack)
1. Wait until dialog box closed.

### Analog Joystick Selection & Calibration
1. Place M5Stack face's up on a horizontal position like a table.
1. Push Button-B for Menu.
1. Select "Select Joystick: Analog".
1. Select "Calibrate Joystick". (Don't move thumb stick)
1. After "Move stick vertically and holizontally." message, move thumb stick vertically and holizontally several times.

### I2C Joystick Selection & Calibration
1. Place M5Stack face's up on a horizontal position like a table.
1. Push Button-B for Menu.
1. Select "Select Joystick: I2C".
1. Select "Calibrate Joystick". (Don't move thumb stick)
1. After "Move stick vertically and holizontally." message, move thumb stick vertically and holizontally several times.

# Licence
GNU GENERAL PUBLIC LICENSE Version 3

# Supported functions
* M5Apple2 uses SD-Updater. Booting M5Stack with Button-A pressed will start SD-Updater.
* Supports saving file(s). It writes file(s) on SD Card directly.

# Unsupported functions
* Sounds
* Serial Communication
* Printers

