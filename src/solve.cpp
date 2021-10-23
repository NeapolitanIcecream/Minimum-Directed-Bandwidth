#include "vertex.h"
#include "iostream"
#include <queue>
#include "instruction_io.h"
#include "graph_builder.h"
#include"string.h"
#include <vector>
using namespace std;
using namespace mdb;

void Visit(mdb::VertexPtr v, std::unique_ptr<mdb::Graph> &gptr);
std::deque<mdb::VertexPtr> visitedButHasUnvisitedNext;

void CheckCopy(uint32_t order, std::unique_ptr<mdb::Graph> &gptr) {
  int32_t checkOrder = static_cast<int32_t>(order) - static_cast<int32_t>(gptr->maxBandwidth) + 1;
  if (order == 4) {
    std::cout << "zbh" << endl;
  }
  if (checkOrder >= 0 && gptr->visitedVertices[checkOrder]->MoreThanOneNextUnvisited()) {
     auto copy = mdb::Copy(visitedButHasUnvisitedNext.front());
     visitedButHasUnvisitedNext.pop_front();
     Visit(copy, gptr);
  }
}

void Visit(mdb::VertexPtr v, std::unique_ptr<mdb::Graph> &gptr) {
  v->Visit();
  gptr->visitedVertices.push_back(v);
  if (!v->AllNextVisited()) {
    visitedButHasUnvisitedNext.push_back(v);
  }
  CheckCopy(v->order, gptr);
  while (!visitedButHasUnvisitedNext.empty()) {
    bool found = false;
    for (auto i : visitedButHasUnvisitedNext) {
       for (auto j : i->GetUnvisitedNext()) {
         if (j->AllPrevVisited()) {
           Visit(j, gptr);
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

void scheduling(std::unique_ptr<mdb::Graph> &gptr) {
  while (true) {
    bool changed = false;
    for (auto &v : gptr->GetAllVertexPtr()) {
      if (v->AllPrevVisited() && !v->HasVisited()) {
        Visit(v, gptr);
        changed = true;
      }
    }
    if (!changed) {
      break;
    }
  }
}

int main() {
  string str;
  cout << "Choose Fixed From file?";
  cin >> str;
  // Build Graph
  if (str == "YES") {
    string fileName;
    cout << "Input file name:" << std::endl;
    cin >> fileName;
    uint32_t bandWidth;
    cout << "Input bit:" << std::endl;
    cin >> bandWidth;
    InputInstructionManager newInput(*new InputInstructionManager(fileName));
    FixedGraphBuilder fg(newInput);
    GraphPtr gptr = fg.BuildGraph(bandWidth);
    scheduling(gptr);
    gptr->Dump();
  } else {
    RandomGraphBuilder rg(20);
    uint32_t bandWidth;
    cout << "Input bit:" << std::endl;
    cin >> bandWidth;
    GraphPtr gptr = rg.BuildGraph(bandWidth);
    gptr->Print();
  }
}
