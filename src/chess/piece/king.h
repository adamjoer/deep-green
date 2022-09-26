#pragma once

#include "piece.h"

namespace Chess {

    class King final : public Piece {
    public:
        King() = delete;

        explicit King(Color color)
            : Piece(color, color == Color::White ? L'♔' : L'♚') {
        }

        ~King() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const BoardState &state) const override;
    };
}
