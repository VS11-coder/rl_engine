#pragma once
#include <cmath>
#include <memory>
#include <vector>
using namespace std;

class MCTSNode {
public:
  int visits;
  double valuesum;
  shared_ptr<MCTSNode> parent;
  vector<shared_ptr<MCTSNode>> children;
  int moveid;
  MCTSNode(shared_ptr<MCTSNode> parentnode, int move)
      : visits(0), valuesum(0.0), parent(parentnode), moveid(move) {}
  double get_ucb1(double exploration_param = 1.414) const {
    if (visits == 0)
      return 1e9;
    double exploitation = valuesum / visits;
    double exploration = exploration_param * sqrt(log(parent->visits) / visits);
    return exploitation + exploration;
  }
  bool isleaf() const { return children.empty(); }
};
