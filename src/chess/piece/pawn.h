#pragma once

#include "piece.h"

namespace Chess {

    class Pawn final : public Piece {
    public:
        Pawn() = delete;

        explicit Pawn(Color color)
            : Piece(color, color == Color::White ? L'♙' : L'♟') {
        }

        ~Pawn() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;

    private:
        // It is detected if the pawn has moved by checking it's still standing
        // on its start row, since pawns can't move along a row.
        static const int START_ROW = 1;
    };
}
