#pragma once

#include "piece.h"

namespace Chess {

    class Knight final : public Piece {
    public:
        Knight() = delete;

        explicit Knight(Color color)
            : Piece(color, color == Color::White ? L'♘' : L'♞') {
        }

        ~Knight() override = default;

        [[nodiscard]]
        std::vector<Position> possibleMoves(const BoardState &state) const override;
    };
}
