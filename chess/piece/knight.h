#pragma once

#include "piece.h"

namespace Chess {

    class Knight final : public Piece {
    public:
        Knight() = delete;

        Knight(Color color, Position position)
            : Piece(color, position) {
        }

        ~Knight() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;

        [[nodiscard]] wchar_t symbol() const override {
            return (color == Color::White ? L'\u2658' : L'\u265E');
        }
    };
}
