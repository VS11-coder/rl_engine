#pragma once
#include "game_state.h"
#include <vector>
#include <memory>
#include <cstdint>
using namespace std;

class Connect4State : public GameState {

private:
    uint64_t current_positon;
    uint64_t mask;
    int moves_played;
    bool check_win(uint64_t bitboard) const;

public:
    Connect4State();
    Connect4State(uint64_t current_pos, uint64_t board_mask, int moves);
    int current_player() const override;
    vector<int> get_legal_moves() const override;
    shared_ptr<GameState> apply_move(int move_id) const override;
    bool is_terminal() const override;
    double get_reward(int player_id) const override;
};

