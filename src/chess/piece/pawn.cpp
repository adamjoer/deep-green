#include "pawn.h"

namespace Chess {
    std::vector<Position> Pawn::possibleMoves(const BoardState &state) const {
        // TODO: Handle "en passant" and "promotion" moves... somehow

        if ((this->color == Color::White && state.position.second == state.board.size() - 1) ||
            (this->color == Color::Black && state.position.second == 0))
            return {};

        std::vector<Position> moves;

        const int yOffset = (this->color == Color::White) ? 1 : -1;

        if (!state.board[state.position.first][state.position.second + yOffset]) {
            moves.emplace_back(state.position.first, state.position.second + yOffset);

            if (state.position.second == startRow
                && !state.board[state.position.first][state.position.second + (2 * yOffset)])
                moves.emplace_back(state.position.first, state.position.second + (2 * yOffset));
        }

        if (state.position.first > 0) {
            const auto &leftDiagonalSquare
                = state.board[state.position.first - 1][state.position.second + yOffset];
            if (leftDiagonalSquare && leftDiagonalSquare.color != this->color)
                moves.emplace_back(state.position.first - 1, state.position.second + yOffset);
        }

        if (state.position.first < state.board.size() - 1) {
            const auto &rightDiagonalSquare
                = state.board[state.position.first + 1][state.position.second + yOffset];
            if (rightDiagonalSquare && rightDiagonalSquare.color != this->color)
                moves.emplace_back(state.position.first + 1, state.position.second + yOffset);
        }

        return moves;
    }
}
