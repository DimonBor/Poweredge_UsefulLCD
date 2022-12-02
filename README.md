# Poweredge_UsefulLCD
C++ rewrite of https://github.com/ExactExampl/Poweredge_UsefulLCD

Tested on Dell Poweredge R410

## Installing
Simply run 
```bash
make install
``` 
and binary will be in `/usr/local/bin`. Systemd daemon will be created and enabled during **install**.

## Usage
Before building you can specify `TIMETOSLEEP` which defines timeout for screen changing.
```C++
/* main.cpp */
#define TIME_TO_SLEEP 2
```
Also, there is **-f** option available. It receives 3 chars that define sequence of information
- m — Memory
- c — CPU load
- t — average temperature for each CPU 

## Examples
Following will run program with default parameters:
``` bash
UsefulLCD
```
To specify custom sequence of screens do this:
``` bash
UsefulLCD -f cmt
``` 
## Uninstalling 
```
make uninstall
```
