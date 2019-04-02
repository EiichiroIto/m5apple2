# M5Apple2
M5Apple2はM5Stack用のApple ][エミュレータです。LinApple(http://linapple.sourceforge.net/ )を移植して作られています。

## ビルドのために必要なもの
* Visual Studio Code (https://code.visualstudio.com/)
* Platform IO (vscode extension)
* M5Stack Library
* M5Stack SD-Updater (https://github.com/tobozo/M5Stack-SD-Updater)
* M5Stack OnScreenKeyboard (https://github.com/lovyan03/M5Stack_OnScreenKeyboard)
* MPU6050_tockn (https://github.com/Tockn/MPU6050_tockn)

## ビルドの準備
* Visual Studio Code はインストール済とします。
* Visual Studio Code の拡張機能として PlatformIO が設定済とします。
* PIO Home の Libraries タブで、M5Stack SD-Updater, M5Stack OnScreenKeyboard, MPU6050_tockn をインストールします。
* 適当なフォルダで、以下のように、このgitレポジトリをクローンしておきます。

`git clone https://github.com/EiichiroIto/m5apple2.git`

## ビルド
1. PIO Home の Home タブで、New Project を選びます。
1. Name に m5apple2 と入力します。
1. Board では M5Stack FIRE を選びます。
1. Framework では Arduino を選びます。
1. Location のチェックを外し、クローンしたgitレポジトリを含むフォルダを開きます。(gitレポジトリのフォルダではなく、その親フォルダを開く)
1. Finish ボタンを押す。
1. エクスプローラに表示された M5STACK のワークスペースで src フォルダ内の main.cpp を削除します。
1. ターミナルメニューからタスクの実行を選び、一覧から PlatformIO:Build （または、 PlatformIO:Upload）を選んでビルド（アップロード）します。

