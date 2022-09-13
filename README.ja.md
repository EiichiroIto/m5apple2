# M5Apple2
M5Apple2はM5Stack用のApple ][エミュレータです。LinApple(http://linapple.sourceforge.net/ )を移植して作られています。

## 遊ぶために必要なもの
* M5Stack CORE2 (https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit)
* マイクロSDカード (16GB推奨。32GB以上は読み込めない場合があるようです)
* Apple ][のディスクイメージ
* (強く推奨) M5Stack Official CardKB Mini Keyboard. (キーボード系のゲームでは必須です）
* (推奨) M5Stack I2C Joystick Unit.

## 遊ぶための準備
* M5Apple2 のファームウェアをビルドするか、あるいはダウンロードします。
* M5Apple2 のファームウェアを M5Stack に転送します。
* Apple ][のディスクイメージを、マイクロSDカードの "/m5apple2" フォルダにコピーします。
* マイクロSDカードをM5Stackに挿入します。

## 遊ぶ
1. M5Stackの電源を入れると自動的にM5Apple2が起動します。
1. Bボタンを押してメニューを表示させます。
1. "Insert Disk #1"を選んでBボタンを押し、適当なディスクイメージを選びます。
1. "Reset"を選んでエミュレータを起動します。

## キーボード
* M5Stack公式CardKBミニキーボードに対応しています。

オンスクリーンキーボードには対応していません。

## ジョイスティック
* Fireに搭載されている加速度センサを使った、ジョイスティックエミュレーションに対応しています。
* M5Stack 公式のI2Cジョイスティックに対応しています。
* ジョイスティックのボタン(A,B)は、M5StackのAボタンとCボタンを使います。

Groveの(アナログ)ジョイスティックには対応していません。

### 加速度センサジョイスティックの選択とキャリブレーション
1. M5Stackの液晶面を上にして水平なところに置いてください。
1. Bボタンを押してメニューを表示させます。
1. "Select Joystick: Accelerometer" を選びます。 (M5Stackを動かさないように)
1. "Calibrate Joystick" を選びます。 (M5Stackを動かさないように)
1. ダイアログボックスが消えるまで少し待ちます。

### I2Cジョイスティックの選択とキャリブレーション
1. M5Stackの液晶面を上にして水平なところに置いてください。
1. Bボタンを押してメニューを表示させます。
1. "Select Joystick: I2C" を選びます。
1. "Calibrate Joystick" を選びます。
1. "Move stick vertically and holizontally."というメッセージがでたら、スティックを上下左右に何度か動かしてください。

# ライセンス
GNU GENERAL PUBLIC LICENSE Version 3

# サポート
* ファイル保存に対応しました。SDカード上のファイルを書き換えるので気をつけてください。


# 未サポート
* サウンド
* シリアル通信
* プリンタ
* アナログジョイスティック
* ソフトウェアキーボード
* SD-Updater
