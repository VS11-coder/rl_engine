#include "connect4.h"
#include <cstdint>
#include <sys/types.h>

using namespace std;

Connect4State::Connect4State() : current_position(0), mask(0), moves_played(0) {}

Connect4State::Connect4State(uint64_t current_pos, uint64_t board_mask, int moves) : current_position(current_pos), mask(board_mask), moves_played(moves) {}

int Connect4State::current_player() const {
    return (moves_played % 2 == 0 ) ? 1 : 2;
}

bool Connect4State::check_win(uint64_t bitboard) const {
    uint64_t m = bitboard & (bitboard >> 7);
    if( m & (m>>14) ) return true;
    m = bitboard & (bitboard>>6);
    if( m & (m>>12)) return true;
    m = bitboard & (bitboard>>8);
    if( m & (m>>16)) return true;
    m = bitboard & (bitboard>>1);
    if( m & (m>>2) ) return true;
    return false;
}

vector<int> Connect4State::get_legal_moves() const {
    vector<int> moves;
    for( int col=0; col<7; col++ ) {
        if( (mask & (1ULL << (col*7+5))) == 0 ) {
            moves.push_back(col);
        }
    }
    return moves;
}

shared_ptr<GameState> Connect4State::apply_move(int move_id) const {
    uint64_t new_move = 0;
    for( int row=0; row<6; row++ ) {
        uint64_t bit = 1ULL << (move_id * 7 + row);
        if( (mask & bit ) == 0 ) {
            new_move = bit;
            break;
        }
    }
    uint64_t new_mask = mask | new_move;
    uint64_t new_position = current_position ^ new_mask;
    return make_shared<Connect4State>(new_position,new_mask, moves_played+1);
}

bool Connect4State::is_terminal() const {
    uint64_t opponent_position = current_position ^ mask;
    return moves_played == 42 || check_win(opponent_position);
}

double Connect4State::get_reward(int player_id) const {
    uint64_t opponent_position = current_position ^ mask;
    if( check_win(opponent_position)) {
        return (current_player() == player_id) ? -1.0 : 1.0;
    }
    return 0.0;
}

