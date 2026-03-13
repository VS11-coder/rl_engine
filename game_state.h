#pragma once
#include <memory>
#include <vector>
using namespace std;

class GameState {
public:
    virtual ~GameState() = default;
    virtual int current_player() const = 0;
    virtual vector<int> get_legal_moves() const = 0;
    virtual shared_ptr<GameState> apply_move(int move_id) const = 0;
    virtual bool is_terminal() const = 0;
    virtual double get_reward(int player_id) const = 0;
};

