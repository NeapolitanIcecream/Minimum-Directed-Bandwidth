#include "vertex.h"
#include <dueue>

std::vector<mdb::VertexPtr> graph;
std::deque<mdb::VertexPtr> visitedButHasUnvisitedNext;
std::vector<mdb::VertexPtr> visited;
const uint32_t maxBandwidth = 8;
void CheckCopy(uint32_t order);
void Visit(mdb::VertexPtr v);

void CheckCopy(uint32_t order) {
    auto checkOrder = order - maxBandwidth + 1;
    if (checkOrder >= 0 && visited[checkOrder] == visitedButHasUnvisitedNext.front() &&
        visitedButHasUnvisitedNext.front()->MoreThanOneNextUnvisited()) {
        auto copy = mdb::Copy(visitedButHasUnvisitedNext.front());
        visitedButHasUnvisitedNext.pop_front();
        Visit(copy);
    }
}

void Visit(mdb::VertexPtr v) {
    v->Visit();
    visited.push_back(v);
    if (!v->AllNextVisited()) {
        visitedButHasUnvisitedNext.push_back(v);
    }
    CheckCopy(v->order);
    while (!visitedButHasUnvisitedNext.empty()) {
        bool found = false;
        for (auto i : visitedButHasUnvisitedNext) {
            for (auto j : i->GetUnvisitedNext()) {
                if (j->AllPrevVisited()) {
                    Visit(j);
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
        if (!found) {
            break;
        }
    }
}

void Solve() {
    while (true) {
        bool found = false;
        for (auto i : graph) {
            if (i->AllPrevVisited() && !i->visited) {
                Visit(i);
                found = true;
            }
        }
        if (!found) {
            break;
        }
    }
}

int main() {
    return 0;
}
