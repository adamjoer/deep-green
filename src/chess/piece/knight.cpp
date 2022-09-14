#include "knight.h"

#include <array>
#include <stdexcept>

namespace Chess {
    std::vector<Position> Knight::possibleMoves(const Board &board) const {
        assert(board[mPosition.first][mPosition.second].get() == this);

        std::vector<Position> moves;

        const std::array<Position, 8> possibleMoves{
            {
                {mPosition.first + 1, mPosition.second + 2},
                {mPosition.first + 2, mPosition.second + 1},
                {mPosition.first + 2, mPosition.second - 1},
                {mPosition.first + 1, mPosition.second - 2},
                {mPosition.first - 1, mPosition.second - 2},
                {mPosition.first - 2, mPosition.second - 1},
                {mPosition.first - 2, mPosition.second + 1},
                {mPosition.first - 1, mPosition.second + 2},
            }
        };

        for (const auto &move: possibleMoves) {
            try {
                const auto &square = board.at(move.first).at(move.second);

                if (!square || square->color != color)
                    moves.push_back(move);

            } catch (std::out_of_range &) {
                continue;
            }
        }

        return moves;
    }
}
