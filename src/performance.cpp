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

            return utilization;
        }
};