#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
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

void OutputInstructionManager::DumpToFile(const std::string &fileName) {
    std::ofstream file(fileName, std::ios::out);
    if (!file.is_open()) {
        std::cout << "write instructions file fail\n";
        std::exit(-1);
    }
    std::function<void(const mdb::VertexPtr&)> visitor = [&file, this](const mdb::VertexPtr &vertex) {
        if (vertex->id < instructions.size()) {
            std::string &oldInstruction = instructions[vertex->id];
            size_t i = 0;
            size_t parentNum = 0;
            while (i < oldInstruction.length()) {
                std::string str = "";
                if (oldInstruction[i] == 't' && (i + 1) < oldInstruction.length() && oldInstruction[i + 1] == '#') {
                    str.append("t#");
                    i += 2;
                    while (i < oldInstruction.length() && oldInstruction[i] >= '0' && oldInstruction[i] <= '9') {
                        i++;
                    }
                    std::stringstream ss;
                    ss << vertex->order - vertex->prev[parentNum++]->order;
                    str.append(ss.str());
                } else {
                    str.push_back(oldInstruction[i]);
                    i++;
                }
                file << str << std::endl;
            }
        } else {
            file << "COPY " << " t#" << (vertex->order - vertex->prev[0]->order) << std::endl;
        }
    };
    graph.ForeachVisitedVertex(visitor);
    file.close();
}
