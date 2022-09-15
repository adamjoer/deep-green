#pragma once

#include "piece.h"

namespace Chess {

    class King final : public Piece {
    public:
        King() = delete;

        King(Color color, Position position)
            : Piece(color, position, color == Color::White ? L'\u2654' : L'\u265A') {
        }

        ~King() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;
    };
}
