#pragma once

#include <utility>
#include <array>
#include <vector>
#include <memory>

#include "piece.h"

namespace Chess {

    class Game {
    public:
        Game();

        ~Game() = default;

        void resetGame();

        [[nodiscard]] std::vector<Position> getPossibleMoves(Position position) const;

        void move(const Position &from, const Position &to);

        void printBoard(const std::vector<Position> &possibleMoves = {}) const;

    private:
        Board mBoard{nullptr};

        Color mCurrentTurn = Color::White;
    };
}
