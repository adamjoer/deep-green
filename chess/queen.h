#pragma once

#include "piece.h"

namespace Chess {

    class Queen final : public Piece {
    public:
        Queen() = delete;

        Queen(Color color, Position position)
            : Piece(color, position) {
        }

        ~Queen() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;

        [[nodiscard]] wchar_t symbol() const override {
            return (color == Color::White ? L'\u2655' : L'\u265B');
        }
    };
}
