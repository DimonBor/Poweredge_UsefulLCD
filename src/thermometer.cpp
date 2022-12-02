#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

// Class for defining temperature of each CPU
class CPUtemp {
    public:
        int CPUid;
        float temp;
        CPUtemp(int newCPUid, int newTemp) {
            this->CPUid = newCPUid; this->temp = (float)newTemp/1000;
        }
};

// Global Thermometer
class Thermometer {
    private:
        int CPUs = 0;
    public:
        Thermometer() { // defining number of CPUs
            for (const auto & entry : std::filesystem::directory_iterator("/sys/devices/platform/")) {
                std::string entryPath = entry.path();
                entryPath = entryPath.substr(22);
                if (entryPath.substr(0,8) == "coretemp") {
                    this->CPUs++;
                }
            }
        }

        // Returning CPU temp from file
        int getTempFromFile (const std::string filePath){
            int outputTemp;
            std::ifstream inputFile;
            inputFile.open(filePath, std::ios::in);
            if (!inputFile.is_open()) {
                std::cout << "Error reading sensors!!!" << std::endl;
                exit(0);
            }
            inputFile >> outputTemp;
            inputFile.close();
            return outputTemp;
        }

        int getNumberOfCPUs() { return this->CPUs; }

        std::vector<CPUtemp> getTemps() { // Checking temps for each CPU
            std::vector<CPUtemp> output;
            for (int i = 0; i < CPUs; i++) {
                float summaryTemp = 0; 
                int coreCounter = 0, hwmonID;
                // Iterating CPUs
                for (const auto & entry : std::filesystem::directory_iterator("/sys/devices/platform/coretemp."+std::to_string(i)+"/hwmon/")) {
                    std::string entryPath = entry.path();
                    hwmonID = std::stoi(entryPath.substr(44));
                }
                for (const auto & entry : std::filesystem::directory_iterator("/sys/devices/platform/coretemp."+std::to_string(i)+"/hwmon/hwmon"+std::to_string(hwmonID)+"/")) {
                    std::string entryPath = entry.path();
                    // Iterating cores
                    if (entryPath.substr(46, 4) == "temp") {
                        std::size_t _pos = entryPath.find("_");
                        if (entryPath.substr(_pos+1, 5) == "input") { 
                            summaryTemp += getTempFromFile(entryPath);
                            coreCounter++;
                        }
                    }
                }
                output.push_back(CPUtemp(i, summaryTemp/coreCounter));
            }
            return output;
        }
};

