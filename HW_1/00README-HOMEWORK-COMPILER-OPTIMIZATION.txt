Homework 3. The effect of compiler optimization on execution time of programs



This homework is for you to experiment with different compiler optimizations.
The given files P10-1.c and P10-2.c contains two programs written 
by a novice student. You do not need
study the program. Instead, you are asked to compile the program with/without
the optimization flags -O1, -O2, and -O3. Measure the execution time of each
version and write a report on the execution time.

A sample report is provided in the file "A taste of compiler optimization.doc".
Just fill in your name and various execution time. Also remember to write a 
conclusion of your experiment. The conclusion should be concerned with the effects
of compiler optimizations on the execution time.

Your job includes the following three things:

(1) copy all the files to a directory
(2) run the following two commands:
	$ make P1 P1-O1 P1-O2 P1-O3
	$ make P2 P2-O1 P2-O2 P2-O3
(3) Write a report based on the sample reports. Change necessary information
and various execution time. Finally, write a conclusion.

--
For program P10-1, you may add an additional option -sort.
For program P10-1, 第一個打line_set1.txt 然後還有line_set1.txt -sort
For program P10-2, 第二個只有line_set1.txt
總共有三種計算


——

IP-209-93:Homework on Compiler Optimization sillytoe$ make P1
cc P10-1.c -o P10-1
./P10-1 line_set1.txt
Start computing...
total overlapping length: 21832333064013
computing time use: 76.285 sec


IP-209-93:Homework on Compiler Optimization sillytoe$ make P1-O1
cc -O1 P10-1.c -o P10-1-O1
./P10-1-O1 line_set1.txt
Start computing...
total overlapping length: 21832333064013
computing time use: 40.061 sec


IP-209-93:Homework on Compiler Optimization sillytoe$ make P1-O2
cc -O2 P10-1.c -o P10-1-O2
./P10-1-O2 line_set1.txt
Start computing...
total overlapping length: 21832333064013
computing time use: 29.577 sec


IP-209-93:Homework on Compiler Optimization sillytoe$ make P1-O3
cc -O3 P10-1.c -o P10-1-O3
./P10-1-O3 line_set1.txt
Start computing...
total overlapping length: 21832333064013
computing time use: 30.685 sec



IP-209-93:Homework on Compiler Optimization sillytoe$ make P1-O3
cc -O3 P10-1.c -o P10-1-O3
./P10-1-O3 line_set1.txt
Start computing...
total overlapping length: 21832333064013
computing time use: 29.676 sec




——

IP-209-93: sillytoe$ make P2 P2-O1 P2-O2 P2-O3 
cc P10-2.c -o P10-2
./P10-2 line_set1.txt
Start sorting...
sorting time use: 15.164 sec
Start computing...
total overlapping length: 21832333064013
computing time use: 64.621 sec
total time use: 79.785 sec

cc -O1 P10-2.c -o P10-2-O1
./P10-2-O1 line_set1.txt
Start sorting...
sorting time use: 6.014 sec
Start computing...
total overlapping length: 21832333064013
computing time use: 34.390 sec
total time use: 40.404 sec

cc -O2 P10-2.c -o P10-2-O2
./P10-2-O2 line_set1.txt
Start sorting...
sorting time use: 5.552 sec
Start computing...
total overlapping length: 21832333064013
computing time use: 20.043 sec
total time use: 25.595 sec

cc -O3 P10-2.c -o P10-2-O3
./P10-2-O3 line_set1.txt
Start sorting...
sorting time use: 5.508 sec
Start computing...
total overlapping length: 21832333064013
computing time use: 20.224 sec
total time use: 25.732 sec


——