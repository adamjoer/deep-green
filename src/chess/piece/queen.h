#pragma once

#include "piece.h"

namespace Chess {

    class Queen final : public Piece {
    public:
        Queen() = delete;

        explicit Queen(Color color)
            : Piece(color, color == Color::White ? L'♕' : L'♛') {
        }

        ~Queen() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;
    };
}
