# __Occurences of words and letters__

# *A text file or multiple files containing English text will be sent to the program, and the program should give an output (word or character frequencies) as a horizontal bar chart printed on the terminal.*

## __NOTE :__ There are different control and input arguments for the program. According to the arguments, the program changes its behaviour and give result in the expected output.

<br/>

First you have to compile the programe
```bash
gcc -Wall program.c -o program
```

> ## Control arguments for the program

<br/>

### __File name/ File Names__
*The program is able to accept any number of file names in any order. File names will not
start with ‘-’. eg: -file.txt*

### __Number of rows in the chart__
*The argument specifies the number of rows in the bar chart. It should be given as -l 10 where 10 is
the limit. It can be any positive integer. A number should always follow the -l argument. The pair
can be in any place of the arguments list.*

### __Scaled option__
*When --scaled argument is given, the first row of the graph should fully occupy the max print width.
Any other row should scale to be matched with first row scale factor*

### __Word/Character toggle__
*The program can analyse two modes of frequencies, characters and words. If the output should be
given as words, the -w should be given whereas -c argument will give character frequency output.*

>Default options

*The program must take at least one file name to work. All other arguments are optional. If not
given, the program will work as non-scaled, will output frequencies for words and limit the output
rows to 10.*

<br/>


__NOTE__:<br/>
>__--You can put arguments in any order__ <br/>
__--Text files should be in same directory where the program is located__

<br/>

# *Methods to run the program*          

With 2 text files

<br/>

```bash
./program -c --scaled -l 5 example1.txt example2.txt
```
```bash
./program -c example1.txt example2.txt
```
```bash
./program example1.txt example2.txt
```
```bash
./program example1.txt
```
```bash
./program --scaled example1.txt
```
```bash
./program -w --scaled example1.txt
```
```bash
./program --scaled -c example1.txt
```
```bash
./program --scaled example1.txt -c
```

*and many more*

