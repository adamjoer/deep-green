#include "rook.h"

namespace Chess {
    std::vector<Position> Rook::possibleMoves(const Board &board) const {
        assert(board[this->position.first][this->position.second].get() == this);

        std::vector<Position> moves;

        // FIXME: Do this so the same for-loop isn't copy-pasted four times

        // Up
        for (int y = this->position.second + 1; y < BOARD_SIZE; ++y) {
            const auto &square = board[this->position.first][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(this->position.first, y);
                break;
            }

            moves.emplace_back(this->position.first, y);
        }

        // Down
        for (int y = this->position.second - 1; y >= 0; --y) {
            const auto &square = board[this->position.first][y];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(this->position.first, y);
                break;
            }

            moves.emplace_back(this->position.first, y);
        }

        // Left
        for (int x = this->position.first - 1; x >= 0; --x) {
            const auto &square = board[x][this->position.second];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, this->position.second);
                break;
            }

            moves.emplace_back(x, this->position.second);
        }

        // Right
        for (int x = this->position.first + 1; x < BOARD_SIZE; ++x) {
            const auto &square = board[x][this->position.second];
            if (square) {
                if (square->color != this->color)
                    moves.emplace_back(x, this->position.second);
                break;
            }

            moves.emplace_back(x, this->position.second);
        }

        return moves;
    }
}
