#include "pawn.h"

namespace Chess {
    std::vector<Position> Pawn::possibleMoves(const Board &board) const {
        assert(board[this->position.first][this->position.second].get() == this);

        // TODO: Handle "en passant" and "promotion" moves... somehow

        if ((this->color == Color::White && this->position.second == BOARD_SIZE - 1) ||
            (this->color == Color::Black && this->position.second == 0))
            return {};

        std::vector<Position> moves;

        const int yOffset = (this->color == Color::White) ? 1 : -1;

        if (!board[this->position.first][this->position.second + yOffset]) {
            moves.emplace_back(this->position.first, this->position.second + yOffset);

            if (!this->hasMoved
                && !board[this->position.first][this->position.second + (2 * yOffset)])
                moves.emplace_back(this->position.first, this->position.second + (2 * yOffset));
        }

        if (this->position.first > 0) {
            const auto &leftDiagonalSquare
                = board[this->position.first - 1][this->position.second + yOffset];
            if (leftDiagonalSquare && leftDiagonalSquare->color != this->color)
                moves.emplace_back(this->position.first - 1, this->position.second + yOffset);
        }

        if (this->position.first < BOARD_SIZE - 1) {
            const auto &rightDiagonalSquare
                = board[this->position.first + 1][this->position.second + yOffset];
            if (rightDiagonalSquare && rightDiagonalSquare->color != this->color)
                moves.emplace_back(this->position.first + 1, this->position.second + yOffset);
        }

        return moves;
    }

    void Pawn::move(const Position &destination) {
        if (destination == this->position)
            return;

        Piece::move(destination);
        this->hasMoved = true;
    }
}
