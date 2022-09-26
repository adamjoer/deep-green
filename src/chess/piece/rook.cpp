#include "rook.h"

namespace Chess {
    std::vector<Position> Rook::possibleMoves(const BoardState &state) const {
        std::vector<Position> moves;

        // FIXME: Do this so the same for-loop isn't copy-pasted four times

        // Up
        for (int y = state.position.second + 1; y < state.board.size(); ++y) {
            const auto &square = state.board[state.position.first][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(state.position.first, y);
                break;
            }

            moves.emplace_back(state.position.first, y);
        }

        // Down
        for (int y = state.position.second - 1; y >= 0; --y) {
            const auto &square = state.board[state.position.first][y];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(state.position.first, y);
                break;
            }

            moves.emplace_back(state.position.first, y);
        }

        // Left
        for (int x = state.position.first - 1; x >= 0; --x) {
            const auto &square = state.board[x][state.position.second];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, state.position.second);
                break;
            }

            moves.emplace_back(x, state.position.second);
        }

        // Right
        for (int x = state.position.first + 1; x < state.board.size(); ++x) {
            const auto &square = state.board[x][state.position.second];
            if (square) {
                if (square.color != this->color)
                    moves.emplace_back(x, state.position.second);
                break;
            }

            moves.emplace_back(x, state.position.second);
        }

        return moves;
    }
}
