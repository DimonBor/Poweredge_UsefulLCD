#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <numeric>
#include <cstring>
#include <cstdlib>
#include <csignal>

#define TIME_TO_SLEEP 2

#include "performance.cpp"
#include "thermometer.cpp"
#include "lcd.cpp"

const char DEFAULT_SEQ [4] = "mct";
/*
m - Memory
c - CPU load
t - Temps for each CPU 
*/

// Prototypes 
void usage();
void exiting(int);
std::string toString(float);


int main(int argc, char* argv[]) {
    Thermometer mainThermometer;
    PerformanceMetrics mainMetrics;
    LCD mainLCD;

    int* memInfo;
    std::vector<CPUtemp> temps;
    std::string sequence = "";

    signal(SIGTERM, exiting);
    signal(SIGABRT, exiting);

    for (int i = 0; i < argc; i ++) { // Parsing command line args 
        if (!strcmp(argv[i], "-h")) {
            usage();
            return 0;
        } 
        if (!strcmp(argv[i], "-f") && i != argc-1) {
            sequence = argv[i+1];
        }
    }

    if (sequence == "") sequence = DEFAULT_SEQ;

    while (true) { // Main loop 
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
                            " t: " + std::to_string(temps[i].getTemp()) + "C"
                        );
                        if (i != mainThermometer.getNumberOfCPUs()-1) sleep(TIME_TO_SLEEP);
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


void usage(){ // -h command
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

void exiting(int signum) { // Write Hostname to LCD on Exit 
    LCD exitingLCD;
    std::string output;
    std::ifstream inputFile;

    inputFile.open("/etc/hostname", std::ios::in);
    if (!inputFile.is_open()) {
        output = "Hostname";
    }
    else {
        std::getline(inputFile, output);
    }
    inputFile.close();
    output = output.substr(0, 13);
    exitingLCD.writeToLCD(output);
    exit(signum);
}