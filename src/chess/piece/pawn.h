#pragma once

#include "piece.h"

namespace Chess {

    class Pawn final : public Piece {
    public:
        Pawn() = delete;

        Pawn(Color color, Position position)
            : Piece(color, position) {
        }

        ~Pawn() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;

        [[nodiscard]] wchar_t symbol() const override {
            return (color == Color::White ? L'\u2659' : L'\u265F');
        }
    };
}
