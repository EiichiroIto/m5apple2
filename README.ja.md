# M5Apple2
M5Apple2はM5Stack用のApple ][エミュレータです。LinApple(http://linapple.sourceforge.net/)を移植して作られています。

## 遊ぶために必要なもの
* M5Stack FIRE (https://docs.m5stack.com/#/en/core/fire)
* マイクロSDカード (16GB推奨。32GB以上は読み込めない場合があるようです)
* Apple ][のディスクイメージ
* (オプション) M5Stack_LovyanLauncher (https://github.com/lovyan03/M5Stack_LovyanLauncher) (ビルドしないで遊ぶなら必須です)
* (オプション) Grove Thumb Joystick (Analog Joystick)
* (オプション) M5Stack Official CardKB Mini Keyboard. (キーボード系のゲームでは必須です）

## ビルドのために必要なもの
* Visual Studio Code (https://code.visualstudio.com/)
* Platform IO (vscode extension)
* M5Stack Library
* M5Stack SD-Updater (https://github.com/tobozo/M5Stack-SD-Updater)
* M5Stack OnScreenKeyboard (https://github.com/lovyan03/M5Stack_OnScreenKeyboard)
* MPU6050_tockn (https://github.com/Tockn/MPU6050_tockn)

## 遊ぶための準備
* M5Apple2をビルドしてM5Stackにファームウェアを転送するか、M5StackにLovyanLauncherをインストールして、このパッケージ内の "bin" フォルダ以下にあるプログラムをコピーします。
* Apple ][のディスクイメージを、マイクロSDカードの "/m5apple2" フォルダにコピーします。
* マイクロSDカードをM5Stackに挿入します。

## 遊ぶ
1. M5Stackの電源を入れると自動的にM5Apple2が起動します。
1. Aボタンを押してメニューを表示させます。
1. "Insert Disk #1"を選んでBボタンを押し、適当なディスクイメージを選びます。
1. "Reset"を選んでエミュレータを起動します。

## キーボード
* らびやん氏のオンスクリーンキーボードに対応しています。
* M5Stack公式CardKBミニキーボードに対応しています。

## ジョイスティック
* Fireに搭載されている加速度センサを使った、ジョイスティックエミュレーションに対応しています。
* Groveの(アナログ)ジョイスティックに対応しています。
* ジョイスティックのボタン(A,B)は、M5StackのBボタンとCボタンを使います。

### 加速度センサジョイスティックのキャリブレーション
1. M5Stackを水平なところに置いてください。
1. Aボタンを押してメニューを表示させます。
1. "Select Joystick: Accelerometer" を選びます。 (M5Stackを動かさないように)
1. "Calibrate Joystick" を選びます。 (M5Stackを動かさないように)
1. ダイアログボックスが消えるまで少し待ちます。

### Analog Joystick Calibration
1. M5Stackを水平なところに置いてください。
1. Aボタンを押してメニューを表示させます。
1. "Select Joystick: Analog" を選びます。
1. "Calibrate Joystick" を選びます。
1. "Move stick vertically and holizontally."というメッセージがでたら、スティックを上下左右に何度か動かしてください。

## SD-Updater
* SD-Updaterに対応しているので、Aボタンを押しながらM5Stackを起動するとSD-Updaterに戻ります。

# 未サポート
* サウンド
* ファイルの保存
* シリアル通信
* プリンタ

# ビルド
1. `git clone https://github.com/EiichiroIto/m5apple2`
1. Run Visual Studio Code and Open m5stack folder.
1. Build and Upload.

