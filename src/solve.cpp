#include "vertex.h"
#include "iostream"
#include <queue>
#include "instruction_io.h"
#include "graph_builder.h"
#include"string.h"
#include <vector>
using namespace std;
using namespace mdb;

void solve(GraphPtr gptr) {
  gptr->initVirtualRoot();
  auto virtualRoot = gptr->virtualRoot->id;
  gptr->BuildSubtreeScore();
  gptr->Visit(virtualRoot);
  while (!gptr->activeVertices.empty()) {
    auto visitWhoseChildrenId = gptr->ChooseWhoseChildren2Visit();
    auto visitWhichId = gptr->ChooseWhich2Visit(visitWhoseChildrenId);
    gptr->Visit(visitWhichId);
  }
}

int main() {
  string str;
  cout << "Input from file? (y/n)";
  cin >> str;
  // Build Graph
  if (str == "y") {
    string fileName;
    cout << "Input file name:" << std::endl;
    cin >> fileName;
    uint32_t bandWidth;
    cout << "Input max bandwidth:" << std::endl;
    cin >> bandWidth;
    InputInstructionManager newInput(*new InputInstructionManager(fileName));
    FixedGraphBuilder fg(newInput);
    GraphPtr gptr = fg.BuildGraph(bandWidth);
    solve(gptr);
    gptr->Dump();
  }
  else {
    RandomGraphBuilder rg(12);
    uint32_t bandWidth;
    cout << "Input max bandwidth:" << std::endl;
    cin >> bandWidth;
    GraphPtr gptr = rg.BuildGraph(bandWidth);
    gptr->Print();
    solve(gptr);
    gptr->Dump();
  }
}
