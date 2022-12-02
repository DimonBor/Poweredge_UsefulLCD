#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>

#include "thermometer.cpp"

int main(int argc, char* argv[]) {
    Thermometer newThermometer;
    std::vector<CPUtemp> temps = newThermometer.getTemps();
    for (int i = 0; i < newThermometer.getNumberOfCPUs(); i++) {
        std::cout << "CPU " << temps[i].CPUid << " average: " << temps[i].temp << std::endl; 
    }
    return 0;
}