class PerformanceMetrics {
    private:
        float idleTime = 0, totalTime = 0;
    public:
        PerformanceMetrics() {  }

        std::string getProcStat (){ // Returns first line of /proc/stat
            std::string output;
            std::ifstream inputFile;
            inputFile.open("/proc/stat", std::ios::in);
            if (!inputFile.is_open()) {
                std::cout << "Error reading /proc/stat!!!" << std::endl;
                exit(0);
            }
            std::getline(inputFile, output);
            output = output.substr(5);
            inputFile.close();
            return output;
        }

        int * getMemInfo (){ // Returns int[2] with 2 values for avail memory and total memory
            std::string memTotalStr, memAvailStr, memFreeStr, tempString;
            static int output [2];

            std::ifstream inputFile;
            inputFile.open("/proc/meminfo", std::ios::in);
            if (!inputFile.is_open()) {
                std::cout << "Error reading /proc/meminfo!!!" << std::endl;
                exit(0);
            }
            std::getline(inputFile, memTotalStr);
            std::getline(inputFile, memFreeStr);
            std::getline(inputFile, memAvailStr);

            inputFile.close();

            std::istringstream memTotalStream(memTotalStr);
            std::vector<std::string> memTotalArray;
            while (getline(memTotalStream, tempString, ' ')) { // parsing Total Memory line 
                if (tempString == "") continue;
                memTotalArray.push_back(tempString);
            }

            std::istringstream memAvailStream(memAvailStr);
            std::vector<std::string> memAvailArray;
            while (getline(memAvailStream, tempString, ' ')) { // parsing Available Memory line 
                if (tempString == "") continue;
                memAvailArray.push_back(tempString);
            }

            int availMemKBytes = std::stoi(memAvailArray[1]);
            int totalMemKBytes = std::stoi(memTotalArray[1]);

            output[0] = (totalMemKBytes - availMemKBytes)/1048576;
            output[1] = totalMemKBytes/1048576; 

            return output;
        }

        int getCPUpercentage() { // Returns CPU utilization in percents 
            // Parsing first line of /proc/stat
            std::string procStat = getProcStat(), tempString;
            std::istringstream procStatStream(procStat);
            std::vector<int> procStatArray;
            while (getline(procStatStream, tempString, ' ')) {
                procStatArray.push_back(std::stoi(tempString));
            }
            // Calculating CPU utilization 
            float newIdleTime = procStatArray[3];
            float newTotalTime = std::accumulate(procStatArray.begin(), procStatArray.end(), 0);
            float idleTimeDelta = newIdleTime - idleTime;
            float totalTimeDelta = newTotalTime - totalTime;
            int utilization = 100.0 * (1.0 - idleTimeDelta / totalTimeDelta);
            this->idleTime = newIdleTime;
            this->totalTime = newTotalTime;

            return utilization;
        }
};