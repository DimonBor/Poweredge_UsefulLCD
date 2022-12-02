#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <numeric>

#include "thermometer.cpp"
#include "performance.cpp"

int main(int argc, char* argv[]) {
    Thermometer newThermometer;
    std::vector<CPUtemp> temps = newThermometer.getTemps();
    
    for (int i = 0; i < newThermometer.getNumberOfCPUs(); i++) {
        std::cout << "CPU " << temps[i].getID() << " average: " << temps[i].getTemp() << std::endl; 
    }

    PerformanceMetrics newMetrics;
    
    while (true) {
        std::cout << newMetrics.getCPUpercentage() << "%" << std::endl;
        sleep(1);
    }
    return 0;
}