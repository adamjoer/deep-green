#pragma once

#include <cassert>

#include <utility>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>

namespace Chess {

    /**
     * Type representing the coordinates of a certain square on the board.
     * These coordinates follow the typical system in chess where origin is in
     * the bottom left corner.
     */
    using Position = std::pair<int, int>;

    enum class Color {
        White,
        Black,
    };

    struct SquareStatus {
        Color color;

        explicit operator bool() const {
            return isEmpty;
        }

    private:
        bool isEmpty;
    };

    struct BoardState {
        std::array<std::array<SquareStatus, 8>, 8> board;
        std::pair<int, int> position;
    };

    class Piece {
    public:
        Piece() = delete;

        explicit Piece(Color color, wchar_t symbol)
            : color(color),
              symbol(symbol) {
        }

        virtual ~Piece() = default;

        /**
         * Check if this piece can move to the given position.
         * Positions where this piece can capture an enemy piece are also included.
         *
         * @param destination Position to check
         * @param state The current state of the board
         * @return Whether this piece can move to 'destination'
         */
        [[nodiscard]] bool canMoveTo(const Position &destination, const BoardState &state) const {

            // FIXME: There is most definitely a more efficient way to check this.
            //        This solution is just the quickest and easiest way to implement
            //        this function right now.
            const auto possibleMoves = this->possibleMoves(state);
            return std::find(possibleMoves.begin(), possibleMoves.end(),
                             destination) != possibleMoves.end();
        }

        // Abstract member functions to be implemented by inheriting pieces

        /**
         * Get all possible positions where this chess piece can move, given the
         * current state of the board.
         * Positions where this piece can capture an enemy piece are also included.
         *
         * @param state The current state of the board
         * @return A vector containing all the positions this piece can move to
         */
        [[nodiscard]] virtual std::vector<Position> possibleMoves(const BoardState &state) const = 0;

        const Color color;

        /**
         * The unicode symbol corresponding to this chess piece.
         * See https://en.wikipedia.org/wiki/Chess_symbols_in_Unicode
         */
        const wchar_t symbol;
    };
}
