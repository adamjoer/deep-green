#pragma once

#include <utility>
#include <array>
#include <vector>
#include <memory>

#include "piece/piece.h"

namespace Chess {

    class Game {
    public:
        Game();

        ~Game() = default;

        void reset();

        [[nodiscard]] std::vector<Position> getPossibleMoves(Position position) const;

        bool move(const Position &from, const Position &to);

        void printBoard(const std::vector<Position> &possibleMoves = {}) const;

        [[nodiscard]] Color turn() const { return this->currentTurn; }

    private:
        Board board{nullptr};

        Color currentTurn = Color::White;
    };
}
