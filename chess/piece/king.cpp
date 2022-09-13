#include "king.h"

#include <algorithm>

namespace Chess {
    std::vector<Position> King::possibleMoves(const Board &board) const {
        // TODO: Handle "castling" move
        
        std::vector<Position> moves;
        
        for (int x = std::max(mPosition.first - 1, 0);
             x <= mPosition.first + 1 && x < BOARD_SIZE; ++x) {

            for (int y = std::max(mPosition.second - 1, 0);
                 y <= mPosition.second + 1 && y < BOARD_SIZE; ++y) {
                if (Position(x, y) == mPosition)
                    continue;

                const auto &square = board[x][y];
                if (!square || square->color != color)
                    moves.emplace_back(x, y);
            }
        }

        return moves;
    }
}
