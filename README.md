# For Japanese
Raspberry Piの温度、CPU使用率、動作周波数をグラフ化します。
日本語の説明は以下に書いています。英語の説明は最小限です。
https://www.thunsuke.com/index.php/blog/thun-tmchk-explanation/


# What is tmchk?
This is only for Raspbian(Raspberry Pi OS).
tmchk is logging your Raspbery Pi system performance and output to graph.
It have 3 parameter : 1,Temperature(deg, Max 100 deg.) 2,CPU frequency(MHz,Max 2500MHx) 3, CPU_usage(%,Max 100)
This tool is possible to check such system performance.

# Usage
usage is very easy. This is syntax of tmchk.
```
tmchk [sampling rate(int/sec)] [logging time(int,min)] [full path of putput png file] (--separate)
```
If you use the "--separate" option, can get 4 graphs below(If don't use "--separate" option, you can get below only one 4.graph).
1. CPU temperature graph
2. CPU frequency graph
3. CPU usage graph
4.  with the above 3 parameters, drawing on one graph (same as ver0.01)
# Dependent package
tmchk is need 2 package.
```
top
gnuplot
```
# Installation
If you have not installed gnuplot , first do below.
```
sudo apt-get install gnuplot
```
tmchk installation
```
git clone https://github.com/T-H-Un/tmchk.git
cd tmchk
make
sudo make install
make clean
```

# Example 
```
tmchk 3 3 /home/pi/test.png
```
Execution result is below.
![rectest](https://user-images.githubusercontent.com/39953146/88944630-f04b1a00-d2c7-11ea-9461-be4a8b2fb425.png)

Use "--separate" option
```
tmchk 1 1 /home/pi/test.png --separate
```
![test002a-2 _CPU_frequency](https://user-images.githubusercontent.com/39953146/90794248-88787400-e347-11ea-8fb6-468bbe67fa7a.png)
CPU frequency graph
![test002a-2 _CPU_temperature](https://user-images.githubusercontent.com/39953146/90794253-89110a80-e347-11ea-8c3b-ef052b98be61.png)
CPU temperature graph
![test002a-2 _CPU_usage](https://user-images.githubusercontent.com/39953146/90794254-89a9a100-e347-11ea-8b3b-d40492b5d4fc.png)
CPU usage graph
![test002a-1](https://user-images.githubusercontent.com/39953146/90794258-89a9a100-e347-11ea-980c-4dfe2d356bae.png)
All system performance graph


