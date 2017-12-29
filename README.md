# top-k_AOFP_pos
テキストデータ等のファイルを入力とし、頻出するパターンの内、長さでtop-k個の長さ、頻度、ポジションを指定したファイルに出力する

# solca
This program is a space-optimal version of grammar compression called FOLCA [1].  
The detail will be showed in A Space-Optimal Grammar Compression [2].  
This program uses [RSDic](https://code.google.com/archive/p/rsdic/) as a part of the data structure.

# コンパイル
    $ cd top-k_OAFP_pos/src/  
    $ make

# 実行方法
    $ ./compress --input_file=string --compressed_file=string --size=int --output_file=string [options] ...
options:
  -i, --input_file         input file name (string)
  -c, --compressed_file    compressed file name (string)
  -s, --size               constant size (int)
  -e, --erase_br           erase line break (bool [=0])
  -o, --output_file        output file name (string)
  -?, --help               print this message

# パラメータ、出力内容等
  --compressed_file  圧縮ファイル
  --size             長さで上位"size"個の頻出パターン
  --output_file      長さで上位"size"個の頻出パターンの長さ、頻度、ポジションを出力 (ただし、最初に出現するポジションは含まない、かつその分実際の頻度より頻度が1少ない)
  出力ファイル内容
  1つのパターンに対して以下の情報を出力
  ****************
  "長さ"
  "頻度"
  "ポジション"
  ...
  "ポジション"(頻度と同じ数ある)
  *****************
  "size"個分以上の情報を出力

# 参考文献
[1] Shirou Maruyama, Yasuo Tabei, Hiroshi Sakamoto, Kunihiko Sadakane:  
Fully-Online Grammar Compression. SPIRE 2013, 218-229.

[2] Yoshimasa Takabatake, Tomohiro I, Hiroshi Sakamoto:  
A Space-Optimal Grammar Compression. ESA2017, to appear.
