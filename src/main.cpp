#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <numeric>
#include <cstring>

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

    float* memInfo;
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
                        "RAM: " + toString(memInfo[0]) + "/" + toString(memInfo[1]) + " GB"
                    );
                    break;
                case 'c':
                    mainLCD.writeToLCD(
                        "CPU: " + toString(mainMetrics.getCPUpercentage()) + "%"
                    );
                    break;
                case 't':
                    temps = mainThermometer.getTemps();
                    for (int i = 0; i < mainThermometer.getNumberOfCPUs(); i++) {
                        mainLCD.writeToLCD(
                            "CPU " + std::to_string(temps[i].getID()) + 
                            " average temp: " + toString(temps[i].getTemp()) + " C"
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


std::string toString(float value) {
    std::string output = std::to_string(value);
    return output.substr(0, output.size()-4);
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