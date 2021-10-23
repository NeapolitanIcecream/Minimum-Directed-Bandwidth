//
// Created by j00518482 on 2021/10/23.
//

#ifndef SRC_GRAPH_BUILDER_H
#define SRC_GRAPH_BUILDER_H
#include "instruction_io.h"
#include "vertex.h"

using GraphPtr = std::unique_ptr<mdb::Graph>;
class GraphBuilder {
public:
    virtual GraphPtr BuildGraph() const = 0;
};

class RandomGraphBuilder : public GraphBuilder {
public:
    RandomGraphBuilder(uint32_t node) : nodeNum(node) {}
    GraphPtr BuildGraph() const override;
private:
    static constexpr uint32_t expectRootNum = 10;
    uint32_t nodeNum = 0;
};

class FixedGraphBuilder : public GraphBuilder {
public:
    explicit FixedGraphBuilder(std::shared_ptr<InstructionManager> &instructions) : instuctionManager(instructions) {}
    GraphPtr BuildGraph() const override;
private:
    std::shared_ptr<InstructionManager> instuctionManager;
};
#endif //SRC_GRAPH_BUILDER_H
