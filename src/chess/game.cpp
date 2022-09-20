#include "game.h"

#ifdef WIN32
#   include <io.h>
#   include <fcntl.h>
#else
#   include <clocale>
#endif

#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "piece/king.h"
#include "piece/queen.h"
#include "piece/rook.h"
#include "piece/bishop.h"
#include "piece/knight.h"
#include "piece/pawn.h"

#define FILL_COLUMN(index, type)                                 \
    this->board[index] = {                                       \
        {                                                        \
            make_unique<type>(Color::White, Position(index, 0)), \
            make_unique<Pawn>(Color::White, Position(index, 1)), \
            nullptr,                                             \
            nullptr,                                             \
            nullptr,                                             \
            nullptr,                                             \
            make_unique<Pawn>(Color::Black, Position(index, 6)), \
            make_unique<type>(Color::Black, Position(index, 7)), \
        }                                                        \
    };

using namespace std;

namespace Chess {

    Game::Game() {

        // Set console mode so wide characters can be shown in "printBoard" function
#ifdef WIN32
        _setmode(_fileno(stdout), _O_WTEXT);
#else
        setlocale(LC_ALL, "en_US.UTF-8");
#endif

        reset();
    }

    /**
     * Reset the game, so all pieces are standing at their starting positions,
     * and it is white's turn to move.
     */
    void Game::reset() {
        this->currentTurn = Color::White;

        FILL_COLUMN(0, Rook)
        FILL_COLUMN(1, Knight)
        FILL_COLUMN(2, Bishop)
        FILL_COLUMN(3, Queen)
        FILL_COLUMN(4, King)
        FILL_COLUMN(5, Bishop)
        FILL_COLUMN(6, Knight)
        FILL_COLUMN(7, Rook)
    }

    /**
     * Get all the positions to which the piece standing at 'position' can move.
     *
     * @param position The position of the piece whose possible moves will be returned
     * @return A vector containing all the positions the piece can move to
     */
    vector <Position> Game::getPossibleMoves(const Position position) const {
        const auto &piece = this->board.at(position.first).at(position.second);
        if (!piece)
            throw invalid_argument("Position does not contain chess piece");

        return piece->possibleMoves(this->board);
    }

    /**
     * Move the piece standing at the 'from' position to the 'to' position.
     * If there is a piece already standing on the 'to' position, that piece
     * is captured, and is therefore removed from the chess board.
     *
     * @param from The position where the moving piece is standing
     * @param to The position to which the piece will move
     * @return Whether the piece at the 'from' position was moved
     */
    bool Game::move(const Position &from, const Position &to) {
        auto &piece = this->board.at(from.first).at(from.second);
        if (!piece)
            throw invalid_argument("Position does not contain chess piece");

        if (piece->color != this->currentTurn)
            return false;

        if (!piece->canMoveTo(to, this->board))
            return false;

        piece->move(to);

        this->board[to.first][to.second] = std::move(piece);
        this->board[from.first][from.second] = nullptr;

        this->currentTurn = this->currentTurn == Color::White ? Color::Black : Color::White;

        // TODO: Check for checkmate

        return true;
    }

    /**
     * Print the current state of the chess board to console.
     * If possibleMoves contain any valid positions, they are highlighted in red.
     *
     * @param possibleMoves Vector containing positions to be highlighted
     */
    void Game::printBoard(const vector <Position> &possibleMoves) const {

        // In this function we print arcane ANSI runes so the console output
        // has pretty colors.
        // See this very helpful SO-answer for a bit of info:
        // https://stackoverflow.com/a/33206814/18713517

        wcout << ' ';
        for (int i = 0; i < BOARD_SIZE; ++i)
            wcout << (char) ('a' + i);
        wcout << '\n';

        bool evenRow = false;
        for (int y = BOARD_SIZE - 1; y >= 0; --y) {
            bool evenColumn = evenRow;

            wcout << (y + 1);
            for (int x = 0; x < BOARD_SIZE; ++x) {

                if (find(possibleMoves.begin(), possibleMoves.end(),
                         Position(x, y)) != possibleMoves.end())
                    // Possible moves are highlighted in red
                    wcout << "\033[41m";

                else if (evenColumn)
                    // Light squares are blue
                    wcout << "\033[44m";

                else
                    // Dark squares are default console color
                    wcout << "\033[0m";

                const auto &square = this->board[x][y];
                wcout << (square ? square->symbol : L' ');

                evenColumn = !evenColumn;
            }

            wcout << "\033[0m";
            wcout << (y + 1);
            wcout << '\n';
            evenRow = !evenRow;
        }

        wcout << ' ';
        for (int i = 0; i < BOARD_SIZE; ++i)
            wcout << (char) ('a' + i);
        wcout << '\n';
    }
}