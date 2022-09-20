#include "knight.h"

#include <array>
#include <stdexcept>

namespace Chess {
    std::vector<Position> Knight::possibleMoves(const Board &board) const {
        assert(board[this->position.first][this->position.second].get() == this);

        std::vector<Position> moves;

        const std::array<Position, 8> possibleMoves{
            {
                {this->position.first + 1, this->position.second + 2},
                {this->position.first + 2, this->position.second + 1},
                {this->position.first + 2, this->position.second - 1},
                {this->position.first + 1, this->position.second - 2},
                {this->position.first - 1, this->position.second - 2},
                {this->position.first - 2, this->position.second - 1},
                {this->position.first - 2, this->position.second + 1},
                {this->position.first - 1, this->position.second + 2},
            }
        };

        for (const auto &move: possibleMoves) {
            try {
                const auto &square = board.at(move.first).at(move.second);

                if (!square || square->color != this->color)
                    moves.push_back(move);

            } catch (std::out_of_range &) {
                continue;
            }
        }

        return moves;
    }
}