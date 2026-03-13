#include "mcts_agent.h"
#include "game_state.h"
#include <memory>
#include <random>
using namespace std;

shared_ptr<MCTSNode> MCTSAgent::select(shared_ptr<MCTSNode> node) {
    while(!node->isleaf()) {
        shared_ptr<MCTSNode> best_child = nullptr;
        double max_ucb = -1e9;
        for( auto& child : node->children ) {
            double ucb = child->get_ucb1();
            if( ucb > max_ucb ) {
                max_ucb = ucb;
                best_child = child;
            }
        }
        node = best_child;
    }
    return node;
}

void MCTSAgent::expand(shared_ptr<MCTSNode> node, const shared_ptr<GameState>& state ) {
    auto legal_moves = state->get_legal_moves();
    for( int move : legal_moves ) {
        node->children.push_back(make_shared<MCTSNode>(node,move));
    }
}

double MCTSAgent::simulate(shared_ptr<GameState> state, int root_player) {
    static mt19937 rng(random_device{}());
    while(!state->is_terminal()) {
        auto legal_moves = state->get_legal_moves();
        if( legal_moves.empty() ) break;
        uniform_int_distribution<int> dist(0,legal_moves.size()-1);
        int random_move = legal_moves[dist(rng)];
        state = state->apply_move(random_move);
    }
    return state->get_reward(root_player);
}

void MCTSAgent::backpropagate(shared_ptr<MCTSNode> node, double reward) {
    while(node != nullptr) {
        node->visits += 1;
        node->valuesum += reward;
        node = node->parent;
    }
}

int MCTSAgent::get_best_move(shared_ptr<GameState> root_state ) {
    auto root_node = make_shared<MCTSNode>(nullptr,-1);
    int root_player = root_state->current_player();
    for( int i=0; i<num_iterations; i++ ) {
        auto node = select(root_node);
        if( node->visits == 0 && !root_state->is_terminal() ) {
            expand(node,root_state);
        }
        double reward = simulate(root_state,root_player);
        backpropagate(node,reward);
    }
    int best_move = -1;
    int max_visits = -1;
    for( auto& child : root_node->children ) {
        if( child->visits > max_visits) {
            max_visits = child->visits;
            best_move = child->moveid;
        }
    }
    return best_move;
}

