#pragma once

#include "piece.h"

namespace Chess {

    class Pawn final : public Piece {
    public:
        Pawn() = delete;

        explicit Pawn(Color color)
            : Piece(color, color == Color::White ? L'♙' : L'♟'),
              startRow(color == Color::White ? START_ROW_WHITE : START_ROW_BLACK) {
        }

        ~Pawn() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;

    private:
        // It is detected if the pawn has moved by checking it's still standing
        // on its start row, since pawns can't move along a row.
        static const int START_ROW_WHITE = 1;
        static const int START_ROW_BLACK = 6;

        const int startRow;
    };
}
