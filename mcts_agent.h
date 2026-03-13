#pragma once
#include "mcts_node.h"
#include "game_state.h"
#include <memory>
#include <vector>
using namespace std;

class MCTSAgent {
private:
    int num_iterations;
    shared_ptr<MCTSNode> select(shared_ptr<MCTSNode> node);
    void expand(shared_ptr<MCTSNode> node, const shared_ptr<GameState>& state);
    double simulate(shared_ptr<GameState> state, int root_player);
    void backpropagate(shared_ptr<MCTSNode> node, double reward);
public:
    MCTSAgent(int iterations = 1000 ) : num_iterations(iterations) {} 
    int get_best_move(shared_ptr<GameState> root_state);
};

