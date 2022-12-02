float round(float var) {
    char str[10];
    sprintf(str, "%.2f", var);
    sscanf(str, "%f", &var);
    return var;
}

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

        float * getMemInfo (){ // Returns float[2] with 2 values for avail memory and total memory
            std::string memTotalStr, memAvailStr, memFreeStr, tempString;
            static float output [2];

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

            float availMemKBytes = std::stoi(memAvailArray[1]);
            float totalMemKBytes = std::stoi(memTotalArray[1]);

            output[0] = round((totalMemKBytes - availMemKBytes)/1048576);
            output[1] = round(totalMemKBytes/1048576); 

            return output;
        }

        float getCPUpercentage() { // Returns CPU utilization in percents 
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
            float utilization = 100.0 * (1.0 - idleTimeDelta / totalTimeDelta);
            this->idleTime = newIdleTime;
            this->totalTime = newTotalTime;

            return round(utilization);
        }
};