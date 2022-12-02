class LCD { // Class for interaction with LCD
    public:
        LCD() { }
        
        void writeToLCD(std::string textToWrite) {
            std::vector<char> charToWrite(textToWrite.c_str(), textToWrite.c_str() + textToWrite.size() + 1);
            std::string charRawValues("ipmitool raw 0x6 0x58 193 0x0 0x0");

            if (charToWrite.size() == 0 || charToWrite.size() > 14) {
                std::cout << "Wrong text size!!!" << std::endl;
                exit(0);
            }

            for (int i = 0; i < charToWrite.size(); i++) 
                charRawValues += " 0x" + std::to_string(int(charToWrite[i]));

            const char* command = charRawValues.c_str();

            system(command);
            system("ipmitool raw 0x6 0x58 0xc2 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0");
        }
};