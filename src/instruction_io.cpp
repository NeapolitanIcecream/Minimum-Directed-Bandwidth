#include <iostream>
#include <fstream>
#include <algorithm>
#include "instruction_io.h"
void InputInstructionManager::ResolveFile(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cout << "read instructions file fail\n";
        std::exit(-1);
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line.find_first_not_of(" ") == std::string::npos) {
            continue;
        }
        instructions.emplace_back(line);
    }
    file.close();
#if IO_DEBUG
    for (auto &str : instructions) {
        std::cout << str << std::endl;
    }
#endif
}

void OutputInstructionManager::DumpToFile(const std::string &fileName) const {
    std::ofstream file(fileName, std::ios::out);
    if (!file.is_open()) {
        std::cout << "write instructions file fail\n";
        std::exit(-1);
    }
    for (auto &instruct : instructions) {
        file << instruct << std::endl;
    }
    file.close();
}
