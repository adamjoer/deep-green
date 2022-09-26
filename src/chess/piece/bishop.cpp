#include "bishop.h"

namespace Chess {
    std::vector<Position> Bishop::possibleMoves(const BoardState &state) const {

        std::vector<Position> moves;

        // FIXME: Do this so the same for-loop isn't copy-pasted four times

        // Right and up
        for (int x = state.position.first + 1, y = state.position.second + 1;
             x < state.board.size() && y < state.board.size(); ++x, ++y) {
            const auto &square = state.board[x][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Right and down
        for (int x = state.position.first + 1, y = state.position.second - 1;
             x < state.board.size() && y >= 0; ++x, --y) {
            const auto &square = state.board[x][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Left and down
        for (int x = state.position.first - 1, y = state.position.second - 1;
             x >= 0 && y >= 0; --x, --y) {
            const auto &square = state.board[x][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Left and up
        for (int x = state.position.first - 1, y = state.position.second + 1;
             x >= 0 && y < state.board.size(); --x, ++y) {
            const auto &square = state.board[x][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        return moves;
    }
}
