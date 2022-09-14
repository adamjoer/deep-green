#include "bishop.h"

namespace Chess {
    std::vector<Position> Bishop::possibleMoves(const Board &board) const {
        assert(board[mPosition.first][mPosition.second].get() == this);

        std::vector<Position> moves;

        // FIXME: Do this so the same for-loop isn't copy-pasted four times

        // Right and up
        for (int x = mPosition.first + 1, y = mPosition.second + 1;
             x < BOARD_SIZE && y < BOARD_SIZE; ++x, ++y) {
            const auto &square = board[x][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Right and down
        for (int x = mPosition.first + 1, y = mPosition.second - 1;
             x < BOARD_SIZE && y >= 0; ++x, --y) {
            const auto &square = board[x][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Left and down
        for (int x = mPosition.first - 1, y = mPosition.second - 1;
             x >= 0 && y >= 0; --x, --y) {
            const auto &square = board[x][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        // Left and up
        for (int x = mPosition.first - 1, y = mPosition.second + 1;
             x >= 0 && y < BOARD_SIZE; --x, ++y) {
            const auto &square = board[x][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, y);
                break;
            }

            moves.emplace_back(x, y);
        }

        return moves;
    }
}
