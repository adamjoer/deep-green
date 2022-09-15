#pragma once

#include "piece.h"

namespace Chess {

    class Knight final : public Piece {
    public:
        Knight() = delete;

        Knight(Color color, Position position)
            : Piece(color, position, color == Color::White ? L'\u2658' : L'\u265E') {
        }

        ~Knight() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;
    };
}
