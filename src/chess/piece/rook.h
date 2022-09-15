#pragma once

#include "piece.h"

namespace Chess {

    class Rook final : public Piece {
    public:
        Rook() = delete;

        Rook(Color color, Position position)
            : Piece(color, position, color == Color::White ? L'\u2656' : L'\u265C') {
        }

        ~Rook() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;
    };
}
