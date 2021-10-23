//
// Created by j00518482 on 2021/10/23.
//
#include <sstream>
#include "graph_builder.h"
#include <random>
#include <set>
#include <queue>
GraphPtr FixedGraphBuilder::BuildGraph() const {
    GraphPtr graph(new mdb::Graph(instuctionManager->GetSize()));
    uint32_t idx = 0;
    auto visitor = [&graph, &idx](const std::string &instruct) {
//        size_t isGetInstruct = instruct.find("GET");
//        if (isGetInstruct != std::string::npos) {
//            idx++;
//            return;
//        }
//        size_t isSetInstruct = instruct.find("SET");
//        if (isSetInstruct != std::string::npos) {
//            size_t biasIndex = instruct.find("t#");
//            if (biasIndex == std::string::npos) {
//                std::cout << "SET instruction doesn't give a bias" << std::endl;
//                std::exit(-1);
//            }
//            size_t biasStart = biasIndex + 2;
//            std::string biasStr = "";
//            for (size_t i = biasStart; i != instruct.size() && (instruct[i] >= '0' && instruct[i] <= '9'); ++i) {
//                biasStr.push_back(instruct[i]);
//            }
//            std::stringstream ss;
//            ss << biasStr;
//            uint32_t bias = 0;
//            ss >> bias;
//            graph->AddEdge(idx - bias, idx);
//            idx++;
//            return;
//        }
        size_t current = idx++;
        size_t biasIndex = instruct.find("t#");
        if (biasIndex == std::string::npos) {
            return;
        }
        size_t biasStart = biasIndex + 2;
        std::string biasStr = "";
        for (size_t i = biasStart; i != instruct.size() && (instruct[i] >= '0' && instruct[i] <= '9'); ++i) {
            biasStr.push_back(instruct[i]);
        }
        std::stringstream ss;
        ss << biasStr;
        uint32_t bias = 0;
        ss >> bias;
        graph->AddEdge(current, current - bias);
        std::string subStr = instruct.substr(biasStart, instruct.size() - biasStart);
        biasIndex = subStr.find("t#");
        if (biasIndex == std::string::npos) {
            return;
        }
        biasStart = biasIndex + 2;
        biasStr = "";
        for (size_t i = biasStart; i != instruct.size() && (instruct[i] >= '0' && instruct[i] <= '9'); ++i) {
            biasStr.push_back(instruct[i]);
        }
        ss << biasStr;
        bias = 0;
        ss >> bias;
        graph->AddEdge(current, current - bias);
    };
    return graph;
}

GraphPtr RandomGraphBuilder::BuildGraph() const {
    if (nodeNum < expectRootNum) {
        std::cout << "node num is too small\n";
        std::exit(-1);
    }
    GraphPtr graph(new mdb::Graph(nodeNum));
    for (uint32_t i = 0; i < nodeNum - expectRootNum; ++i) {
        uint32_t current = nodeNum -1 - i;
        uint32_t parentsNum = (static_cast<uint32_t>(std::rand()) % 2) + 1;
        uint32_t parents[2];
        for (uint32_t j = 0; j < parentsNum; ++j) {
            parents[j] = std::rand() % current;
            if (j == 1 && parents[1] == parents[0]) {
                parents[j] = parents[j] == 0 ? 1 : parents[j] - 1;
            }
            graph->AddEdge(parents[j], current);
        }
    }
    return graph;
}