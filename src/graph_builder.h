//
// Created by j00518482 on 2021/10/23.
//

#ifndef SRC_GRAPH_BUILDER_H
#define SRC_GRAPH_BUILDER_H
#include <vector>
#include "instruction_io.h"
#include "vertex.h"

using GraphPtr = std::shared_ptr<mdb::Graph>;
class GraphBuilder {
public:
    virtual GraphPtr BuildGraph(uint32_t maxBandWitdth) const = 0;
};

class RandomGraphBuilder : public GraphBuilder {
public:
    RandomGraphBuilder(uint32_t node) : nodeNum(node) {}
    GraphPtr BuildGraph(uint32_t maxBandWidth) const override;
private:
    static constexpr uint32_t expectRootNum = 4;
    uint32_t nodeNum = 0;
};

class FixedGraphBuilder : public GraphBuilder {
public:
    explicit FixedGraphBuilder(InstructionManager &instructions) : instuctionManager(&instructions) {}
    GraphPtr BuildGraph(uint32_t maxBandWidth) const override;
private:
    InstructionManager *instuctionManager;
};
#endif //SRC_GRAPH_BUILDER_H
