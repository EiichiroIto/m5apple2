# M5Apple2
M5Apple2 is an Apple ][ emulator for M5Stack microcontroller, based on LinApple (http://linapple.sourceforge.net/).

[日本語](https://github.com/EiichiroIto/m5apple2/blob/master/README.ja.md)

https://raw.githubusercontent.com/EiichiroIto/m5apple2/master/bin/jpg/M5Apple2.jpg

## Prerequisites for Play
* M5Stack CORE2 (https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit)
* Micro SD Card (TF Card)
* Apple ][ Disk Image
* (Highly Recommended) M5Stack Official CardKB Mini Keyboard.
* (Recommended) M5Stack I2C Joystick Unit.

## Preparation for Play
* Build the firmware or download it.
* Burn M5Apple2 firmware to your M5Stack.
* Copy some Apple ][ Disk Image(s) to the "/m5apple2" folder on Micro SD Card.
* Insert the Micro SD Card to the M5Stack.

## Play
1. Turn the M5Stack power on then automatically start.
1. Push Button-B for Menu.
1. Select "Insert Disk #1" and Select a Disk Image.
1. Select "Reset" to boot the Apple ][ emulator.

## Keyboard
* Support for CardKB Mini Keyboard.

The software keyboard (M5Stack OnScreenKeyboard) is no longer supported.

## Joystick
* Support for on-board Accelerometer Joystick Emulation.
* Support for M5Stack I2C Joystick.
* Use Button-A and Button-C on the M5Stack for Joystick buttons.

The analog joystick (Grove Thumb Joystick) is no longer supported.

### Accelerometer Joystick Selection & Calibration
1. Place the M5Stack face's up on a horizontal position like a table.
1. Push Button-B for Menu.
1. Select "Select Joystick: Accelerometer".
1. Select "Calibrate Joystick". (Don't move the M5Stack)
1. Wait until dialog box closed.

### I2C Joystick Selection & Calibration
1. Place the M5Stack face's up on a horizontal position like a table.
1. Push Button-B for Menu.
1. Select "Select Joystick: I2C".
1. Select "Calibrate Joystick". (Don't move thumb stick)
1. After "Move stick vertically and holizontally." message, move thumb stick vertically and holizontally several times.

# Licence
GNU GENERAL PUBLIC LICENSE Version 3

# Supported functions
* Supports saving file(s). It writes file(s) on SD Card directly.

The booting system (SD-Updater) is no longer supported.

# Unsupported functions
* Sounds
* Serial Communication
* Printers
* Analog Joystick
* Software keyboard
* SD-Updator

