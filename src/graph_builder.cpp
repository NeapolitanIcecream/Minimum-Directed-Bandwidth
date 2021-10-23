//
// Created by j00518482 on 2021/10/23.
//
#include <sstream>
#include "graph_builder.h"
#include <random>
#include <queue>
GraphPtr FixedGraphBuilder::BuildGraph(uint32_t maxBandWidth) const {
    GraphPtr graph(new mdb::Graph(instuctionManager->GetSize(), maxBandWidth));
    uint32_t idx = 0;
    std::function<void(const std::string&)> visitor = [&graph, &idx](const std::string &instruct) {
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
        graph->AddEdge(current - bias, current);
        std::string subStr = instruct.substr(biasStart, instruct.size() - biasStart);
        biasIndex = subStr.find("t#");
        if (biasIndex == std::string::npos) {
            return;
        }
        biasStart = biasIndex + 2;
        biasStr = "";
        for (size_t i = biasStart; i != subStr.length() && (subStr[i] >= '0' && subStr[i] <= '9'); ++i) {
            biasStr.push_back(subStr[i]);
        }
        std::stringstream ss1;
        ss1 << biasStr;
        bias = 0;
        ss1 >> bias;
        graph->AddEdge(current - bias, current);
    };
    instuctionManager->ForEachInstruction(visitor);
    return graph;
}

GraphPtr RandomGraphBuilder::BuildGraph(uint32_t maxBandWidth) const {
    if (nodeNum < expectRootNum) {
        std::cout << "node num is too small\n";
        std::exit(-1);
    }
    GraphPtr graph(new mdb::Graph(nodeNum, maxBandWidth));
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
