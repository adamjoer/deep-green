#include "king.h"

#include <algorithm>

namespace Chess {
    std::vector<Position> King::possibleMoves(const BoardState &state) const {
        // TODO: Handle "castling" move
        
        std::vector<Position> moves;
        
        for (int x = std::max(state.position.first - 1, 0);
             x <= state.position.first + 1 && x < state.board.size(); ++x) {

            for (int y = std::max(state.position.second - 1, 0);
                 y <= state.position.second + 1 && y < state.board.size(); ++y) {
                if (Position(x, y) == state.position)
                    continue;

                const auto &square = state.board[x][y];
                if (!square || square.color != this->color)
                    moves.emplace_back(x, y);
            }
        }

        return moves;
    }
}
