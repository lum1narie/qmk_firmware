# Ergodox用 新下駄レイアウト(SKK用拡張済み)

## 概要

SKKに対応した新下駄入力用のレイアウト

より詳細に書くと、以下の条件を想定している

+ OSの入力言語は日本語<span id=body1><sup>[1](#foot1)</sup></span>
+ ローマ字入力
+ SKKに対応
    + "C-j", "l", "q", "x", ">"の入力に対応
    + "っ"の入力
        + "かった" -> "katta"のように、"xtu"ではなく子音を重ねる
        + "か<S-っ>た" -> "kaTta"、"かっ<S-た>" -> "katTa"のように、シフト入力のタイミングを厳密にとる
        + 便宜上、2連打以上すると2回目からは"っ"を出力し、以降の入力に影響を及ぼさない<span id=body2><sup>[2](#foot2)</sup></span>
+ SandSを採用
+ 変換の決定を"asdfghjkl"で行う
+ 新下駄の仕様
    + 同時押しの順番は問わない
    + 2打鍵の押し始めのタイミングが一定以内、かつ、
後のキーが押し始められた時に先のキーが離されていないかで判定する
        + このタイミングは設定できるようにする

### 純粋な新下駄入力との違い

以上の要件を満たすため、純粋な新下駄入力に改変を加えた。

+ 右手小指(QWERTYの";")に打鍵面を追加

    ```
    |a|s|d|f|g| | | | | |
    | | | | | | |x| | |#|
    |h|j|k|l|>| | | | | |
    ```

    ただし、">"は通常時">"(主に接頭語の入力キー)、シフト時"Q>"(接尾語の入力キー)を出力するコマンドである
    
## 新下駄レイヤーの使用法

### 使用法 `rules.mk`

```
SRC += shingeta.c shingeta_code.c
LIB_SRC += str_utl.c
```
を追加する

### 使用法 `keymap.c`

+ `#define`を使って`SGTA, SGTS`にレイアウト番号を振って、
`custom_keycodes`に`CS_SGKEY, CS_SGKEY_S`を追加する  
    ただし、`SGTA` < `SGTS`でなければならず、使用時に`SGTA`と`SGTS`の間の番号のレイヤーが
使用されていないことが保障されなくてはならない<span id=body3><sup>[3](#foot3)</sup></span>
+ `keymaps[][][]`の`SGTA, SGTS`を当レイアウトの`keymap.c`からコピペする  
    配列自体をいじりたい場合は下の[手を加えたい人の為の概要](#手を加えたい人の為の概要)を参照
+ TODO:<span id=body4><sup>[4](#foot4)</sup></span> `keymaps[][][]`から`SGTA`レイヤーを呼び出せるようにする
+ `process_record_user`の`CS_SGKEY, CS_SGKEY_S`に関する処理をコピペする  
    この時、`record_event`に関わらずに`shingeta_key()`を呼び出さなければならない
(キーの押下と離しの両方が必要なため)
+ `matrix_init_user()`で`init_shingeta_mode()`を呼び出す
+ TODO:<span id=body4><sup>[4](#foot4)</sup></span> `SGTA`レイヤーの切り替え時に以下の関数を呼び出す
    + `on_enter_shingeta_mode` (`SGTA`レイヤー開始時)
    + `on_exit_shingeta_mode` (`SGTA`レイヤー終了時)

    このため、shingetaモードに入るにあたり、ワンショットキーや一時的長押しのようなレイヤーの切り替え方は想定されておらず、入る時と出る時で別のキーコードによる命令が個別行われることを想定されている。
    `SGTA`と`SGTS`間は一時的長押しによる切り替えであるが、混同しないようにしてほしい。

### 設定

+ `shingeta.h`で`SHINGETA_TAP_TERM`を設定することで、同時押しの判定の時間(ms)を決められる

## 手を加えたい人の為の概要

### キーマップの変更法

以下、1回の`send_string()`のみで実行できない操作、またはシフト等基本的な文字に対する動作において異なった挙動をさせる操作をコマンドと呼ぶ。
また、キー押下時の`event.key.row << 4 + event.key.col`を"物理コード"と呼ぶ


例:

+ カーソル操作を伴うもの
+ `X_INT1`のように`send_string()`に非対応の文字入力
+ シフト時と無シフト時で全く違う文字を入力させるもの

**新下駄用の物理的なキーの変更自体を伴う場合**

新下駄用の通常レイヤーを`SGTA`、シフト用レイヤーを`SGTS`とする。
シフトはSandS用のキーであり、実体は`SGTS`の呼び出しであるものとする。
また、`shingeta_key(record, false)`を呼び出すためのキーコードを`CS_SGKEY`、
`shingeta_key(record, true)`を呼び出すためのキーコードを`CS_SGKEY_S`とする

#### 物理キー変更 `keymap.c`


`keymaps[][][]`に以下の操作を行う

+ 使用したい文字用キーの場所全てについて、
`SGTA`には`CS_SGKEY`、`SGTS`には`CS_SGKEY_S`を追加する
+ SandS用のキーについて、`SGTA`に`LT(SGTS,KC_SPC)`、`SGTS`に`KC_TRNS`を追加する
+ その他通常の配列と同様の操作
    + TODO:<span id=body4><sup>[4](#foot4)</sup></span> <shingetaオン><shingetaオフ>の関数を呼び出すキーを配置する
    + 新下駄に必要な英字入力を空いたスペースに登録する

**コマンドを変更したい場合**

#### コマンド変更 `util/shingeta.c`

コマンドを追加したい場合はコマンドを`int func(char *string, bool is_shifted)`の型の関数で実装し、そのポインタを`functions_for_command[]`に
追加すれば良い

#### コマンド関数の実装

第1引数`string`はコマンドの実体の文字列がコピーされた変数で、大きさは`SHINGETA_STRING_MAX_LENGTH *
                      2`文字分である。<span id=body5><sup>[5](#foot5)</sup></span>自由に変更する事が許される。<span id=body6><sup>[6](#foot6)</sup></span>
第2引数`is_shifted`はコマンドの打鍵時にシフトが押された時`true`である。

基本的には、他のコマンド関数のように

```
string[0] = '\0';
xtu_common(string);
send_string(string);
```

を最初に行う想定である。これは、直前の打鍵が"っ"だった場合の対処である。<span id=body7><sup>[7](#foot7)</sup></span>

shingetaモードの初期化に関わる関数として

+ `init_shingeta_mode()`: 最初に1度だけ呼ばれる
+ `on_enter_shingeta_mode()`: モード開始時に呼ばれる
+ `on_exit_shingeta_mode()`: モード終了時に呼ばれる

があるので、グローバル変数を用いる場合はこれらを活用してほしい。<span id=body8><sup>[8](#foot8)</sup></span>

#### コマンド変更 `shingeta_code.h`

後述の通りに`COMMAND_`から始まるマクロを変更する必要がある

#### コマンド変更 `shingeta_code.c`

`commands[][]`の中身にコマンドを列挙する必要がある

**以下、条件にかかわらずやる必要があるもの**

#### キー変更 `util/shingeta_codelist_maker`

`code.py`に配列を入力して、`shingeta_codelist_maker.py`を実行すれば良い。

`code.py`で設定すべき変数は以下の通りである

+ `shingeta_ergodox_string_dict`: キー配列  
    詳しくは後述
+ `MACRO_COMMAND_LIST`: Cソース上のコマンド名一覧  
    コマンド数を増減させたい時に使うことを想定している
+ `ergodox_code_mapping`: Ergodox上の物理コードの配列と
`shingeta_ergodox_string_dict`の要素である配列の順番の対応表  
    他のキーボードに移植する際に要変更

`shingeta_ergodox_string_dict`の要素について、辞書のキーは同時押しする(中指、薬指等の)シフト用キーの物理コードである。
辞書の値は、`ergodox_code_mapping`の順番に並んだ出力文字である。
ただし、単押しの配列のキーは`NA_NUMBER`である。

同時押しについて、片方が一致する単押しと出力文字が同じだった場合は弾くようにしてあるので、
単押しと同じ文字を記述しても構わないし、差分だけ記述しても構わない。<span id=body9><sup>[9](#foot9)</sup></span>

出力文字について、通常の文字は文字列型であり、コマンドはコマンドの番号(1-indexed)を`Int`型で記述する。

出力されるファイルは以下の通り

+ `shingeta_code.txt`
+ `shingeta_macro.txt`
+ `shingeta_code.csv`

`shingeta_code.txt`と`shingeta_macro.txt`はC言語のソースである。

`shingeta_code.txt`は、これを用いて`shingeta_code.c`の`shingeta_codes[]`の中身をそのまま置き換えれば良い

`shingeta_macro.txt`は、これを用いて`shingeta.h`の該当部分を置き換えれば良い。
その際、`COMMAND_NUM`をコマンド数に書き換える必要がある<span id=body10><sup>[10](#foot10)</sup></span>

`shingeta_code.csv`は出力されたコードの閲覧用であり、C言語のソースには用いない

#### キー変更 `shingeta_code.h`

出力文字を登録する連想配列のサイズは初期値で256になっているが<span id=body11><sup>[11](#foot11)</sup></span>、これで足りない場合は`HASH_MAX`の数値を適当に変更した上で、
`get_hash(c1, c2)`によって定義されているハッシュ関数を新たなサイズに合わせたものに変更する必要がある

----------

<span id=foot1>1</span>: `send_string()`の文字コードに留意する必要がある[↩](#body1)

<span id=foot2>2</span>: 上記の条件のみを満たすようにすると、"っ"を単体で出力する方法が無くなるため[↩](#body2)

<span id=foot3>3</span>: 基本的には連番を想定している[↩](#body3)

<span id=foot4>4</span>: `../lum1narie/keymap.c`で実装した、`tgl_sgta_misc()`や`process_record_user()`の`case CS_OFF_SGTA`ように、レイヤーの変更と`on_`の関数群を紐付ける必要がある[↩](#body4)

<span id=foot5>5</span>: "っ"の処理は"xtu"を先頭につける場合があり、その場合の`send_normal_string()`中で要求される最大文字数に合わせてある[↩](#body5)

<span id=foot6>6</span>: この実装にした理由は思い出せないが、恐らくは全コマンドを`shingeta_send_string()`内にベタ打ちしていた時に`string_to_send`を最初から最後まで使い回していた時の名残りだと考えられる。(実際に`send_normal_string()`が呼び出されるときはそのフローになる)[↩](#body6)

<span id=foot7>7</span>: この共通処理をコマンド呼び出し時に勝手に行わないのは、`xtu_input()`も1つのコマンドとして一般化した処理にしたかったからである[↩](#body7)

<span id=foot8>8</span>: これらの関数の呼び出しは`keymap.c`で正しく行わなければならないので、[使用法 `keymap.c`](#使用法-keymap.c)を参照して各自間違えないように設定してほしい[↩](#body8)

<span id=foot9>9</span>: 完全に一致する2つ組のキーを重複して登録した場合は、処理上で先に読み込まれた方の値を採択するようにしている。もちろん、同じ出力文字が入力されていることを想定している。[↩](#body9)

<span id=foot10>10</span>: TODO: これも本来は`shingeta_macro.txt`に出力されて然るべき内容だと思われる。後で作る。[↩](#body10)

<span id=foot11>11</span>: 現時点で登録数は168である。筆者は配列サイズの0.8倍程までが実用的に使える限度であると聞き及んでいる。[↩](#body11)

