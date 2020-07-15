
**制作 : 多田 瑛貴**

このプログラムは、自らの研究テーマ「**ポインタの走査による新しい空間インデックスの手法についての研究**」にて、 <br>
研究しているアルゴリズムを、C++のプログラムとして試験的に再現したものである。 <br>

本研究における試験実装では、基本的にアルゴリズムの機能を汎用的に使用可能なコンテナクラスとして提供する。 <br>
GUI及び描画ライブラリを使った視覚的シミュレーションなど、様々な形での検証を容易に行うことができるようにするこ
とを目的としているためである。 <br>

なおクラス名である`mulpsi`は「複数ポインタ走査式空間インデックスアルゴリズム」 - " **Mul**ti**p**ointer **s**pacial **i**ndex algorithm " 縮めて**mulp-si**に由来している。 <br>

# 概要

このアルゴリズムは、空間インデックスアルゴリズムの一つとして分類されることを想定している。 <br>
基本機能として

 - 矩形、またはMBR(最小外接矩形)状の空間オブジェクトの管理(挿入・削除)
 - 範囲検索・点検索 (特定の矩形・座標と同じ空間に存在する空間オブジェクトそれぞれとの重複の判定)

を提供する。

そして、今回のアルゴリズムの代表的な特徴としては以下のものがある。

 - 範囲検索について、検索範囲が「動く」場合において、定数時間に限りなく近い時間間隔で処理。 <br>
   (「動く」とは、検索範囲の位置・大きさを示す数値が、少ない値で継続/連続的に変化する状況を指す。)

特に従来`R-tree`で行われていた処理の多くについて、より最適な時間/空間計算量を実現する形で互換することができると考えている。

想定できる主な用途としては以下のものが挙げられる。

 - GIS(地理情報システム)の表示最適化
 - ゲームプログラミングにおけるオブジェクトの描画、及び当たり判定の最適化
 - ベクタグラフィックスの表示最適化


## 研究の動機について

もともとこのアルゴリズムの研究は、過去に自らが作成を試みたゲームにて、 <br>
当たり判定の最適化のために **two-pointer algorithm (通称:しゃくとり法)** を使った手法を考えようと <br>
検討を初めたのが発端となっており、様々な考察を重ねた末の最終形が本アルゴリズムとなっている。 <br>
( 最終的にプロジェクトは、かくしてゲームの作成よりもアルゴリズムの検討がメインになってしまっている。 ) <br>

そのような経緯があるため、mulp-siと two-pointer algorithmは多くの部分で共通点がある。 <br>
( とりわけ、1次元空間の場合だとこの2つはかなり類似していることがわかる。 ) <br>


# 本実装について

## 動作環境

実装言語 : **C++20** <br>
備考 : 本実装では、C++20で導入される予定の標準ライブラリである`<compare>`を用いている。 <br>
従って、gcc-10等の最新のコンパイラでなければ動作しない可能性がある。

## パフォーマンス

*B* : 最善ケース (Best-performance) <br>
*W* : 最悪ケース (Worst-performance) <br>
*A* : 平均パフォーマンス (Average-performance) <br>

#### 時間計算量

**N** : 要素(index)の数

 - 要素の挿入 <br>
  *B*/*W*/*A* : **O(logN)** <br>
  note : 二部探索。 <br>

 - 要素の削除 <br>
  *W*/*A* : **O(N)** <br>
  *B* : **O(1)** <br>
  note : 線形探索。本実装ではデータ構造の都合上、二部探索などによる最適化はできない。 <br>
  
 - 範囲質問 (の解の導出)
   - 最初の処理 (新しく定義された検索範囲内) <br>
    *W*/*A* : **O(N)** <br>
    *B* : **O(1)** <br>
    note : 線形探索。データ構造の都合上、二部探索などによる最適化はできない。 <br>

   - 検索範囲の更新 <br>
    *B*/*A* : **O(1)** <br>
    *W* : **O(N)** <br>
    note : 検索範囲の変化が、(検索対象のオブジェクトの大きさや分散範囲から考えて) <br>
    比較的小さい、あるいは全く無い時には**B**となる。<br>
    逆に、変化が大きいかランダムであれば、基本的に**W**とみなすことができる。 <br>

#### 空間計算量

**N** : 要素(index)の数 <br>
**D** : 空間の次元(例: 2次元(xy空間) -> 2) <br>

 - **O(ND)** <br>

なお、使用する主なコンテナは次のとおりである。(コンテナの種類:サイズ) <br>
`unordered_set`:N + `multiset`:N*2 + `unordered_map`:N

## 課題 (解決及び部分的解決したものを含む)

なお、ここで「従来の実装」として比較されているのは
前回の試験実装 [tp_spacial_index](https://github.com/Perukii/research-theme-A) である。 <br>

#### 解決

 - 新規データの挿入・削除の実装がうまくいかない  <br>
   (**mulp-si-0.1** より実装に成功。解決)  <br>
 - 多次元への展開に重い実装を要する  <br>
   (従来は、一次元処理のみをサポートするクラスを次元数に応じて並べ、  <br>
   それぞれの連携を行う集約クラスを次元別に新しく書く必要があった。)  <br>
   (**mulp-si-0.1** より、単一のクラスで多次元の処理をサポートする設計を試み、実装。解決) <br>

#### 部分的解決

 - コンテナの構造が複雑 <br>
   (従来は、次元別に行われた重複判定の結果をまとめるのに、空間の次元数に応じた複数のhashテーブルを用いる必要があった。) <br>
   (**mulp-si-0.1** より、Y(a)氏のアドバイスを受け、単一のhashテーブルによる実装を実現。) <br>
   (以上より最も大きな課題を回避できたため、これ以上解決を試みる必要性はかなり薄れたが、それでもまだ追求する余地はある。)

#### 未解決

 - **現時点で、アルゴリズムの最低要件を達成する為に必ず解決しなければならない課題は、もはや存在しない。**

#### 今後実装が期待できるもの

 - 判定のさらなる最適化 <br>
   (現状の実装では、次元数x2(始点、終点)個のポインタが(すべての空間オブジェクトの情報を参照しながら) <br>
   それぞれ個別に動作し、その結果をhashテーブルが集約して処理を行うことによって判定が行われている。) <br>
   (次回の実装では、すべての空間オブジェクトの情報を参照するポインタを1つのみに限定し、 <br>
   次のポインタはそこで判定を受けたオブジェクトのみを参照し、その次のポインタもそこで判定を受けたものを参照.... <br>
   という流れで処理することにより、全体としてポインタの移動量が大幅に減らすことができるのではないかと期待している。 <br>
   結局の所計算量は変わらないが、とくに次元数の多い空間、あるいは広大で多くのオブジェクトが散在している空間において <br>
   パフォーマンスの劇的な改善ができると考えている。ただしやはり、実装はかなり難しそうである。) <br>
 - 線形探索処理の削除(二部探索やハッシュ探索による代替の考察)
 - 複数の抽出範囲の定義のサポート
 
 
# パフォーマンス比較

使用ライブラリ : <br>
 - [benchmark](https://github.com/google/benchmark) : ベンチマークのための各種機能の導入に使用
 - [RTree](https://github.com/nushoin/RTree) : 比較するアルゴリズム (R-tree) の導入に使用

### benchmark_test/compare.cpp (2020/07/15 計測)

##### シチュエーション

 - 大きさ1000000 * 1000000 の平面空間内に、 <br>
   大きさ10 * 10 ~ 2000 * 2000 の矩形オブジェクトが**N**個散在
 - 300 * 300の抽出範囲をxy座標 (-300 ,300) から右にスライドさせ、 <br>
   x座標が1加算されるたびに抽出情報を更新
 - x座標が (1000300 ,300)に到達した時処理を終了

**計測環境**

 - OS : Ubuntu 20.04 LTS 64-bit
 - デスクトップ環境 : 無効
 - CPU : Intel® Core™ i5-8250U CPU @ 1.60GHz
 
 

##### 結果

**N = 100000**

| (アルゴリズム) | 最適化なし | R-tree | mulp-si-0.1 |
|------|--------------------|------------------|--------------|
| 計測1 | 506,191,023,358 ns | 1,028,361,753 ns | 52,893,107 ns |
| 計測2 |                    |   961,395,706 ns | 52,994,809 ns |
| 計測3 |                    | 1,028,216,138 ns | 53,065,478 ns |
| 計測4 |                    | 1,099,919,377 ns | 53,044,776 ns |
| 計測5 |                    | 1,260,754,709 ns | 53,070,344 ns |

**N = 10000**

| アルゴリズム | 最適化なし | R-tree | mulp-si-0.1 |
|------|-------------------|----------------|--------------|
| 計測1 | 47,932,202,908 ns | 449,228,618 ns | 45,416,389 ns |
| 計測2 |                   | 345,633,504 ns | 45,401,866 ns |
| 計測3 |                   | 363,493,298 ns | 45,500,681 ns |
| 計測4 |                   | 434,200,859 ns | 45,487,154 ns |
| 計測5 |                   | 433,439,703 ns | 45,369,055 ns |



