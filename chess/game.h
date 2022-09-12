#pragma once

#include <utility>
#include <array>
#include <vector>
#include <memory>

#include "piece.h"

namespace Chess {

    /**
     * Type representing the coordinates of a certain square on the board.
     * These coordinates follow the typical system in chess where origin is in
     * the bottom left corner.
     */
    using Position = std::pair<int, int>;

    class Game {
    public:
        Game();

        ~Game() = default;

        static const int BOARD_SIZE = 8;

        void resetGame();

        [[nodiscard]] std::vector<Position> getPossibleMoves(Position position) const;

        void move(const Position &from, const Position &to);

        void printBoard(const std::vector<Position> &possibleMoves = {}) const;

    private:
        std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE> mBoard
            {nullptr};

        Color mCurrentTurn = Color::White;
    };
}
