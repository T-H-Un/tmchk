# For Japanese
Raspberry Piの温度、CPU使用率、動作周波数をグラフ化します。
日本語の説明は以下のリンクに書いています。ここにある英語の説明は最小限です。
https://www.thunsuke.com/index.php/blog/thun-tmchk-explanation/


# What is tmchk?
This is only for Raspbian(Raspberry Pi OS).
tmchk is logging your Raspbery Pi system performance and output to graph.
It have 3 parameter : 1,Temperature(deg, Max 100 deg.) 2,CPU frequency(MHz,Max 2500MHz) 3, CPU_usage(%,Max 100)
This tool is possible to check such system performance.

# Usage
usage is very easy. This is syntax of tmchk.
```
tmchk [sampling rate(int/sec)] [logging time(int,min)] [full path of putput png file] (options)
```
If you use the "--separate" option, can get 4 graphs below(If don't use "--separate" option, you can get below only one 4.graph).
1. CPU temperature graph
2. CPU frequency graph
3. CPU usage graph
4.  with the above 3 parameters, drawing on one graph (same as ver0.01)

"--nograph" option is not output PNG files.
"--nocsv" option is not output CSV file.
"--realtime_temp" is drawing CPU temperature graph on real time.
"--realtime_usage" is drawing CPU usage graph on real time
"--realtime_freq" is drawing CPU frequency graph on real time
"--realtime_all" is drawing CPU temperature, usage and frequnecy graphs on realtime.
This command can be saw helps.It return usage and options.
```
tmchk --help
```
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
# Uninstall
```
sudo rm /url/local/bin/tmchk
```
If you installed gnuplot with tmchk , run below command.
```
sudo apt-get remove -y --purge gnuplot
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

This is example .csv data file.
```
time,CPU_temperature,CPU_usage,CPU_frequency
0,39.00,11,1000.00
1,39.00,8,1500.00
2,40.00,10,750.00
3,39.00,9,750.00
4,41.00,11,750.00
5,39.00,9,750.00
6,40.00,10,750.00
7,39.00,7,1500.00
8,39.00,7,750.00
9,40.00,8,1500.00
10,39.00,7,750.00
11,40.00,10,750.00
12,39.00,11,1500.00
13,39.00,9,1500.00
14,40.00,10,750.00
15,39.00,10,1500.00
16,39.00,15,750.00
17,39.00,10,750.00
18,39.00,10,750.00
19,40.00,10,1500.00
20,40.00,11,750.00
21,40.00,10,1500.00
22,40.00,10,750.00
23,40.00,7,1000.00
24,39.00,9,1500.00
25,39.00,10,600.00
26,39.00,6,1500.00
27,40.00,9,1500.00
28,40.00,9,1500.00
29,41.00,11,750.00
30,39.00,9,1500.00
31,40.00,9,1500.00
32,40.00,10,750.00
33,39.00,7,750.00
34,40.00,10,1500.00
35,39.00,12,750.00
36,40.00,8,1500.00
37,39.00,9,750.00
38,39.00,10,750.00
39,40.00,7,600.00
40,39.00,6,600.00
41,39.00,7,600.00
42,40.00,6,750.00
43,40.00,5,750.00
44,40.00,6,600.00
45,39.00,32,750.00
46,40.00,11,750.00
47,40.00,9,1500.00
48,39.00,10,600.00
49,39.00,9,750.00
50,40.00,10,1500.00
51,40.00,11,750.00
52,39.00,10,750.00
53,39.00,10,600.00
54,38.00,10,750.00
55,39.00,9,1500.00
56,39.00,11,750.00
57,40.00,7,1500.00
58,39.00,9,1500.00
59,39.00,11,600.00
```

