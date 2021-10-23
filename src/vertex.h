#ifndef SRC_VERTEX_H
#define SRC_VERTEX_H

#include <memory>
#include <vector>
#include <iostream>
#include <list>
#include <optional>
#include <unordered_map>
#include <algorithm>
#
namespace mdb {
class Vertex {
public:
    uint32_t id;
    std::vector<std::shared_ptr<Vertex>> prev;
    std::vector<std::shared_ptr<Vertex>> next;
    bool visited;
    uint32_t order;
    uint32_t unvisitedNextCount;

    Vertex() {
        static uint32_t idCounter = 0;
        id = idCounter++;
        visited = false;
        order = -1;
    }

    void AddPrev(std::shared_ptr<Vertex> v) {
        prev.push_back(v);
    }

    void AddNext(std::shared_ptr<Vertex> v) {
        next.push_back(v);
        unvisitedNextCount++;
    }

    void Print() {
        std::cout << "Vertex " << id << ":" << std::endl;
        for (auto v : prev) {
            std::cout << "  prev: " << v->id << std::endl;
        }
        for (auto v : next) {
            std::cout << "  next: " << v->id << std::endl;
        }
        std::cout << "  visited: " << visited << std::endl;
        std::cout << "  order: " << order << std::endl;
    }

    void SetOrder(uint32_t order) {
        visited = true;
        this->order = order;
    }

    std::vector<std::shared_ptr<Vertex>> GetUnvisitedNext() {
        std::vector<std::shared_ptr<Vertex>> result;
        for (auto v : next) {
            if (!v->visited) {
                result.push_back(v);
            }
        }
        return result;
    }

    void Visit() {
        static uint32_t globalOrder = 0;
        visited = true;
        order = globalOrder++;
        for (auto v : prev) {
            v->unvisitedNextCount--;
        }
    }

    bool AllNextVisited() {
        return unvisitedNextCount == 0;
    }

    bool OneNextUnvisited() {
        return unvisitedNextCount == 1;
    }

    bool MoreThanOneNextUnvisited() {
        return unvisitedNextCount > 1;
    }

    bool AllPrevVisited() {
        for (auto v : prev) {
            if (!v->visited) {
                return false;
            }
        }
        return true;
    }

    // Accessing a child of a truly active node does not reduce the number of active nodes
    bool IsTrueActive() {
        return MoreThanOneNextUnvisited();
    }

    // Accessing a child of a false active node does not increase the number of active nodes
    bool IsFalseActive() {
        return OneNextUnvisited();
    }
};

typedef std::shared_ptr<Vertex> VertexPtr;

namespace {

void RemovePrev(VertexPtr v, VertexPtr prev) {
    for (auto i = v->prev.begin(); i != v->prev.end(); i++) {
        if (*i == prev) {
            v->prev.erase(i);
            break;
        }
    }
}

void RemoveNext(VertexPtr v, VertexPtr next) {
    for (auto i = v->next.begin(); i != v->next.end(); i++) {
        if (*i == next) {
            v->next.erase(i);
            break;
        }
    }
}

void AddEdge(VertexPtr v1, VertexPtr v2) {
    v1->AddNext(v2);
    v2->AddPrev(v1);
}

VertexPtr Copy(VertexPtr v) {
    VertexPtr result(new Vertex());
    result->AddPrev(v);
    for (auto i = v->next.begin(); i != v->next.end(); i++) {
        if (!(*i)->visited) {
            v->next.erase(i);
            RemovePrev(*i, v);
            AddEdge(result, *i);
        }
    }
    v->AddNext(result);
    return result;
}

VertexPtr CreateVertexPtr() {
    VertexPtr result(new Vertex());
    return result;
}

} // namespace

class Graph {
public:
    uint32_t vertexCount;
    std::vector<VertexPtr> vertices;
    std::vector<VertexPtr> visitedVertices;
    std::vector<VertexPtr> activeVertices;
    std::unordered_map<VertexPtr, uint32_t> vertexAncestorsCount;
    std::unordered_map<VertexPtr, uint64_t> subtreeScore;
    uint32_t maxBandwidth;
    uint32_t activeCount;
    uint32_t globalOrder;

    Graph(uint32_t vertexCount, uint32_t maxBandwidth) : vertexCount(vertexCount), maxBandwidth(maxBandwidth) {
        vertices.reserve(vertexCount);
        for (uint32_t i = 0; i < vertexCount; i++) {
            vertices.push_back(CreateVertexPtr());
        }
        activeCount = 0;
        globalOrder = 0;
    }

    void BuildSubtreeScore() {
        for (auto v : vertices) {
            GetSubtreeScore(v);
        }
    }

    uint64_t GetSubtreeScore(VertexPtr v) {
        if (subtreeScore.count(v) != 0) {
            return subtreeScore[v];
        }
        uint64_t result = 0;
        vertexAncestorsCount[v] = 0;
        for (auto i : v->prev) {
            vertexAncestorsCount[v] += vertexAncestorsCount[i];
        }
        for (auto i : v->next) {
            bool allAncestorsCounted = true;
            for (auto j : v->prev) {
                if (vertexAncestorsCount.count(j) == 0) {
                    allAncestorsCounted = false;
                    break;
                }
            }
            if (allAncestorsCounted) {
                result += GetSubtreeScore(i);
            }
        }
        result += vertexAncestorsCount[v];
        subtreeScore[v] = result;
        return result;
    }

    std::optional<uint32_t> ChooseWhoseChildren2VisitFalseActive() {
        for (auto i : activeVertices) {
            if (i->IsFalseActive()) {
                return { i->id };
            }
        }
        return std::nullopt;
    }

    uint32_t ChooseWhoseChildren2Visit4Reduce() {
        uint32_t result = 0;
        for (auto i = 0; i < activeCount; i++) {
            if (subtreeScore[activeVertices[i]] < subtreeScore[activeVertices[result]]) {
                result = i;
            }
        }
        return activeVertices[result]->id;
    }

    double CalculateVisitScore(VertexPtr v) {
        static const uint64_t orderFactor = maxBandwidth * maxBandwidth;
        static const uint64_t subtreeFactor = 1;
        return (globalOrder - v->order) * orderFactor - subtreeScore[v] * subtreeFactor;
    }

    uint32_t ChooseWhoseChildren2VisitBase() {
        uint32_t result = 0;
        double currentScore = CalculateVisitScore(activeVertices[0]);
        for (auto i = 1; i < activeCount; i++) {
            auto newScore = CalculateVisitScore(activeVertices[i]);
            if (newScore > currentScore) {
                result = i;
                currentScore = newScore;
            }
        }
        return activeVertices[result]->id;
    }

    uint32_t ChooseWhoseChildren2Visit() {
        if (NeedCopy(activeVertices[0])) {
            Copy(activeVertices[0]->id);
            return activeVertices[0]->id;
        }
        if (NeedReduce()) {
            auto result = ChooseWhoseChildren2VisitFalseActive();
            if (result.has_value()) {
                return result.value();
            }
            return ChooseWhoseChildren2Visit4Reduce();
        }
        return ChooseWhoseChildren2VisitBase();
    }

    // The subtreeScore of the copy node is trickily designed to be 0, so it will always be accessed first, as expected.
    uint32_t ChooseWhich2Visit(VertexPtr parent) {
        uint64_t minSubtreeScore = std::numeric_limits<uint64_t>::max();
        uint32_t result = 0;
        for (auto i : parent->next) {
            if (!i->visited && subtreeScore[i] < minSubtreeScore) {
                minSubtreeScore = subtreeScore[i];
                result = i->id;
            }
        }
        return result;
    }

    // If a node has more than one unvisited children and its ordinal number is globalOrder - maxBandwidth, it has to be copied
    bool NeedCopy(VertexPtr v) {
        return v->MoreThanOneNextUnvisited() && v->order == globalOrder - maxBandwidth;
    }

    // Determine if the number of active nodes needs to be reduced
    bool NeedReduce() {
        return activeCount >= maxBandwidth / 2;
    }

    // Accessing a child of a truly active node does not reduce the number of active nodes
    bool IsTrueActive(uint32_t vertexId) {
        return vertices[vertexId]->MoreThanOneNextUnvisited();
    }

    // Accessing a child of a false active node does not increase the number of active nodes
    bool IsFalseActive(uint32_t vertexId) {
        return vertices[vertexId]->OneNextUnvisited();
    }

    void Print() {
        for (auto v : vertices) {
            v->Print();
        }
    }

    void Print(uint32_t id) {
        vertices[id]->Print();
    }

    std::vector<VertexPtr> GetUnvisitedNext(uint32_t id) {
        return vertices[id]->GetUnvisitedNext();
    }

    void Visit(uint32_t id) {
        vertices[id]->Visit();
        visitedVertices.push_back(vertices[id]);
        globalOrder++;
        for (auto v : vertices[id]->prev) {
            if (v->AllNextVisited()) {
                activeVertices.erase(std::find(activeVertices.begin(), activeVertices.end(), v));
                activeCount--;
            }
        }
        if (!vertices[id]->AllNextVisited()) {
            activeVertices.push_back(vertices[id]);
            activeCount++;
        }
    }

    void AddEdge(uint32_t v1, uint32_t v2) {
        mdb::AddEdge(vertices[v1], vertices[v2]);
    }

    void AllNextVisited(uint32_t id) {
        vertices[id]->AllNextVisited();
    }

    void OneNextUnvisited(uint32_t id) {
        vertices[id]->OneNextUnvisited();
    }

    void MoreThanOneNextUnvisited(uint32_t id) {
        vertices[id]->MoreThanOneNextUnvisited();
    }

    void AllPrevVisited(uint32_t id) {
        vertices[id]->AllPrevVisited();
    }

    uint32_t Copy(uint32_t v) {
        auto result = mdb::Copy(vertices[v]);
        vertices.push_back(result);
        return result->id;
    }
};
} // namespace mdb
#endif
