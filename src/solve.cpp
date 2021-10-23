#include "vertex.h"
#include "iostream"
#include <queue>
#include "instruction_io.h"
#include "graph_builder.h"
#include"string.h"
#include <vector>
using namespace std;
int main() {
  string fileName;
  cin >> fileName;
  InputInstructionManager newInput(*new InputInstructionManager(fileName));
  FixedGraphBuilder fg(newInput);
  GraphPtr gptr = fg.BuildGraph(3);
  gptr->Print();
}
