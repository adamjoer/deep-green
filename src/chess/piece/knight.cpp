#include "knight.h"

#include <array>
#include <stdexcept>

namespace Chess {
    std::vector<Position> Knight::possibleMoves(const BoardState &state) const {

        std::vector<Position> moves;

        const std::array<Position, 8> possibleMoves{
            {
                {state.position.first + 1, state.position.second + 2},
                {state.position.first + 2, state.position.second + 1},
                {state.position.first + 2, state.position.second - 1},
                {state.position.first + 1, state.position.second - 2},
                {state.position.first - 1, state.position.second - 2},
                {state.position.first - 2, state.position.second - 1},
                {state.position.first - 2, state.position.second + 1},
                {state.position.first - 1, state.position.second + 2},
            }
        };

        for (const auto &move: possibleMoves) {
            try {
                const auto &square = state.board.at(move.first).at(move.second);

                if (!square || square.color != this->color)
                    moves.push_back(move);

            } catch (std::out_of_range &) {
                continue;
            }
        }

        return moves;
    }
}
