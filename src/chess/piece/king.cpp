#include "king.h"

#include <algorithm>

namespace Chess {
    std::vector<Position> King::possibleMoves(const Board &board) const {
        assert(board[this->position.first][this->position.second].get() == this);

        // TODO: Handle "castling" move
        
        std::vector<Position> moves;
        
        for (int x = std::max(this->position.first - 1, 0);
             x <= this->position.first + 1 && x < BOARD_SIZE; ++x) {

            for (int y = std::max(this->position.second - 1, 0);
                 y <= this->position.second + 1 && y < BOARD_SIZE; ++y) {
                if (Position(x, y) == this->position)
                    continue;

                const auto &square = board[x][y];
                if (!square || square->color != this->color)
                    moves.emplace_back(x, y);
            }
        }

        return moves;
    }
}
