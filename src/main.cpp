#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <numeric>
#include <cstring>
#include <cstdlib>

#define TIME_TO_SLEEP 1

#include "performance.cpp"
#include "thermometer.cpp"
#include "lcd.cpp"

const char DEFAULT_SEQ [4] = "mct";
/*
m - Memory
c - CPU load
t - Temps for each CPU 
*/

void usage();
std::string toString(float);


int main(int argc, char* argv[]) {
    Thermometer mainThermometer;
    PerformanceMetrics mainMetrics;
    LCD mainLCD;

    int* memInfo;
    std::vector<CPUtemp> temps;
    std::string sequence = "";

    for (int i = 0; i < argc; i ++) {
        if (!strcmp(argv[i], "-h")) {
            usage();
            return 0;
        } 
        if (!strcmp(argv[i], "-f") && i != argc-1) {
            sequence = argv[i+1];
        }
    }

    if (sequence == "") sequence = DEFAULT_SEQ;

    while (true) {
        for (int i = 0; i < 3; i ++){
            switch((char) sequence[i]) {
                case 'm':
                    memInfo = mainMetrics.getMemInfo();
                    mainLCD.writeToLCD(
                        "RAM: " + std::to_string(memInfo[0]) + "/" + std::to_string(memInfo[1]) + "G"
                    );
                    break;
                case 'c':
                    mainLCD.writeToLCD(
                        "CPU: " + std::to_string(mainMetrics.getCPUpercentage()) + "%"
                    );
                    break;
                case 't':
                    temps = mainThermometer.getTemps();
                    for (int i = 0; i < mainThermometer.getNumberOfCPUs(); i++) {
                        mainLCD.writeToLCD(
                            "CPU" + std::to_string(temps[i].getID()) + 
                            " t: " + std::to_string(temps[i].getTemp()) + " C"
                        );
                        if (i != mainThermometer.getNumberOfCPUs()-1) sleep(1);
                    }
                    break;
                default:
                    break;
            }
            sleep(TIME_TO_SLEEP);
        }
    }
    return 0;
}


void usage(){
    std::cout 
        << "Usage: UsefulLCD [OPTIONS] [ARG]" << std::endl
        << "Options:" << std::endl
        << "-f [char[3]]    Format option to define information sequence" << std::endl
        << "        m - RAM information" << std::endl
        << "        c - CPU utilization" << std::endl
        << "        t - CPU temperature for each CPU" << std::endl
        << "        Another letters will be ignored!" << std::endl
        << std::endl
        << "Examples: " << std::endl
        << "    ./UsefulLCD        <--- will run with default sequence" << std::endl
        << "    ./UsefulLCD -f mtc <--- will run with user defined sequence" << std::endl
        << std::endl;
}