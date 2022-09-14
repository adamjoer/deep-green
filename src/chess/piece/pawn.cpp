#include "pawn.h"

namespace Chess {
    std::vector<Position> Pawn::possibleMoves(const Board &board) const {
        assert(board[mPosition.first][mPosition.second].get() == this);

        // TODO: Handle "en passant" and "promotion" moves... somehow

        if ((this->color == Color::White && mPosition.second == BOARD_SIZE - 1)||
            (this->color == Color::Black && mPosition.second == 0))
            return {};

        std::vector<Position> moves;

        const int yOffset = (this->color == Color::White) ? 1 : -1;

        if (!board[mPosition.first][mPosition.second + yOffset]) {
            moves.emplace_back(mPosition.first, mPosition.second + yOffset);

            if (!mHasMoved
                && !board[mPosition.first][mPosition.second + (2 * yOffset)])
                moves.emplace_back(mPosition.first,mPosition.second + (2 * yOffset));
        }

        const auto &leftDiagonalSquare
            = board[mPosition.first - 1][mPosition.second + yOffset];
        if (leftDiagonalSquare && leftDiagonalSquare->color != this->color)
            moves.emplace_back(mPosition.first - 1, mPosition.second + yOffset);

        const auto &rightDiagonalSquare
            = board[mPosition.first + 1][mPosition.second + yOffset];
        if (rightDiagonalSquare && rightDiagonalSquare->color != this->color)
            moves.emplace_back(mPosition.first + 1, mPosition.second + yOffset);

        return moves;
    }
}
