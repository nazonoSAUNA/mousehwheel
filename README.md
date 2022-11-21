# 拡張編集マウス横ホイールプラグイン
- 拡張編集タイムラインでマウスホイールを動かした際の挙動を変更する

## 動作要件
- Visual C++ 再頒布可能パッケージの2022 x86(32bit)対応の物がインストールされている必要があります【Microsoft Visual C++ 2015-2022 Redistributable(x86)】
- マイクロソフト公式:< https://docs.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist >
- AviUtl解説サイト:< https://scrapbox.io/aviutl/Visual_C++_再頒布可能パッケージ >

- 拡張編集バージョン0.92のみ対応

## 仕様
- 通常のホイール：従来は横スクロール　→　縦スクロールに変更
- Alt押しながらホイール：従来は縦スクロール　→　横スクロールに変更
- (マウスによっては存在する)横ホイールで横スクロールされるようにする
