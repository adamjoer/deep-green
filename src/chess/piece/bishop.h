#pragma once

#include "piece.h"

namespace Chess {

    class Bishop final : public Piece {
    public:
        Bishop() = delete;

        explicit Bishop(Color color)
            : Piece(color, color == Color::White ? L'♗' : L'♝') {
        }

        ~Bishop() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;
    };
}
