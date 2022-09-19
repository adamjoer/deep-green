#pragma once

#include <utility>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>
#include <cassert>

namespace Chess {

    // Forward declaration for Board type
    class Piece;

    /**
     * Type representing the coordinates of a certain square on the board.
     * These coordinates follow the typical system in chess where origin is in
     * the bottom left corner.
     */
    using Position = std::pair<int, int>;

    /**
     * Number of squares on one side of the chess board.
     * The chess board is always square.
     */
    static const int BOARD_SIZE = 8;

    /**
     * Type representing the chess board. If an element in this array is not null,
     * it points to the piece standing at that position.
     * The 'Position' type can be used as index in this array.
     */
    using Board = std::array<std::array<std::shared_ptr<Piece>, BOARD_SIZE>, BOARD_SIZE>;

    enum class Color {
        White,
        Black,
    };

    class Piece {
    public:
        Piece() = delete;

        Piece(Color color, Position position, wchar_t symbol)
            : color(color),
              position(std::move(position)),
              symbol(symbol) {
        }

        virtual ~Piece() = default;

        /**
         * Move this chess piece to the given position.
         *
         * @param destination The position to which this piece will be moved
         */
        virtual void move(const Position &destination) {
            this->position = destination;
        }

        /**
         * Check if this piece can move to the given position.
         * Positions where this piece can capture an enemy piece are also included.
         *
         * @param destination Position to check
         * @param board The current state of the board
         * @return Whether this piece can move to 'destination'
         */
        [[nodiscard]] bool canMoveTo(const Position &destination, const Board &board) const {

            // FIXME: There is most definitely a more efficient way to check this.
            //        This solution is just the quickest and easiest way to implement
            //        this function right now.
            const auto possibleMoves = this->possibleMoves(board);
            return std::find(possibleMoves.begin(), possibleMoves.end(),
                             destination) != possibleMoves.end();
        }

        // Abstract member functions to be implemented by inheriting pieces

        /**
         * Get all possible positions where this chess piece can move, given the
         * current state of the board.
         * Positions where this piece can capture an enemy piece are also included.
         *
         * @param board The current state of the board
         * @return A vector containing all the positions this piece can move to
         */
        [[nodiscard]] virtual std::vector<Position> possibleMoves(const Board &board) const = 0;

        const Color color;

        /**
         * The unicode symbol corresponding to this chess piece.
         * See https://en.wikipedia.org/wiki/Chess_symbols_in_Unicode
         */
        const wchar_t symbol;

    protected:
        Position position;
    };
}
