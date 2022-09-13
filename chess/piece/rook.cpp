#include "rook.h"

namespace Chess {
    std::vector<Position> Rook::possibleMoves(const Board &board) const {
        std::vector<Position> moves;

        // FIXME: Do this so the same for-loop isn't copy-pasted four times

        // Up
        for (int y = mPosition.second + 1; y < BOARD_SIZE; ++y) {
            const auto &square = board[mPosition.first][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(mPosition.first, y);
                break;
            }

            moves.emplace_back(mPosition.first, y);
        }

        // Down
        for (int y = mPosition.second - 1; y >= 0; --y) {
            const auto &square = board[mPosition.first][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(mPosition.first, y);
                break;
            }

            moves.emplace_back(mPosition.first, y);
        }

        // Left
        for (int x = mPosition.first - 1; x >= 0; --x) {
            const auto &square = board[x][mPosition.second];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, mPosition.second);
                break;
            }

            moves.emplace_back(x, mPosition.second);
        }

        // Right
        for (int x = mPosition.first + 1; x < BOARD_SIZE; ++x) {
            const auto &square = board[x][mPosition.second];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, mPosition.second);
                break;
            }

            moves.emplace_back(x, mPosition.second);
        }

        return moves;
    }
}
