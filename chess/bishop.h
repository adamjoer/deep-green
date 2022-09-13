#pragma once

#include "piece.h"

namespace Chess {

    class Bishop final : public Piece {
    public:
        Bishop() = delete;

        Bishop(Color color, Position position)
            : Piece(color, position) {
        }

        ~Bishop() override = default;

        [[nodiscard]] std::vector<Position> possibleMoves(const Board &board) const override;

        [[nodiscard]] wchar_t symbol() const override {
            return (color == Color::White ? L'\u2657' : L'\u265D');
        }
    };
}
