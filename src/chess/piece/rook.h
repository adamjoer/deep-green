#pragma once

#include "piece.h"

namespace Chess {

    class Rook final : public Piece {
    public:
        Rook() = delete;

        explicit Rook(Color color)
            : Piece(color, color == Color::White ? L'♖' : L'♜') {
        }

        ~Rook() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;
    };
}
