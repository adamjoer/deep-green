#include "game.h"

#ifdef WIN32
#   include <io.h>
#   include <fcntl.h>
#else
#   include <clocale>
#endif

#include <algorithm>
#include <iostream>

using namespace std;

namespace Chess {

    // https://stackoverflow.com/a/16957554/18713517
    const int Game::BOARD_SIZE;

    Game::Game() {

        // Set console mode so wide characters can be shown in "printBoard" function
#ifdef WIN32
        _setmode(_fileno(stdout), _O_WTEXT);
#else
        setlocale(LC_ALL, "en_US.UTF-8");
#endif

        resetGame();
    }

    /**
     * Reset the game, so all pieces are standing at their starting positions,
     * and it is white's turn to move.
     */
    void Game::resetGame() {
        mCurrentTurn = Color::White;

        const array<Type, BOARD_SIZE> pieceOrder = {
            Type::Rook, Type::Knight, Type::Bishop, Type::Queen,
            Type::King, Type::Bishop, Type::Knight, Type::Rook
        };

        for (int i = 0; i < BOARD_SIZE; ++i) {
            mBoard[i] = {
                {
                    make_shared<Piece>(pieceOrder[i], Color::White),
                    make_shared<Piece>(Type::Pawn, Color::White),
                    nullptr,
                    nullptr,
                    nullptr,
                    nullptr,
                    make_shared<Piece>(Type::Pawn, Color::Black),
                    make_shared<Piece>(pieceOrder[i], Color::Black),
                }
            };
        }

        // For quick and dirty testing; remember to remove!
        mBoard[3][3] = make_shared<Piece>(Type::Bishop, Color::White);
        mBoard[2][2] = make_shared<Piece>(Type::Pawn, Color::White);
        mBoard[1][5] = make_shared<Piece>(Type::Pawn, Color::Black);
    }

    /**
     * Get all the positions to which the piece standing at 'position' can move.
     *
     * @param position Where the piece whose possible moves will be returned are standing
     * @return A vector containing all the positions the piece can move to
     */
    vector <Position> Game::getPossibleMoves(const Position position) const {
        const auto &piece = mBoard[position.first][position.second];
        if (!piece)
            return {};

        vector<Position> ret;

        switch (piece->type) {
            case Type::King:
                // TODO: Handle "castling" move

                for (int x = max(position.first - 1, 0);
                     x <= position.first + 1 && x < BOARD_SIZE; ++x) {

                    for (int y = max(position.second - 1, 0);
                         y <= position.second + 1 && y < BOARD_SIZE; ++y) {
                        if (Position{x, y} == position)
                            continue;

                        const auto &square = mBoard[x][y];
                        if (!square || square->color != piece->color)
                            ret.emplace_back(x, y);
                    }
                }
                break;

            case Type::Queen:
                // TODO: Implement
                break;

            case Type::Rook:

                // FIXME: Do this so the same for-loop isn't copy-pasted four times

                // Up
                for (int y = position.second + 1; y < BOARD_SIZE; ++y) {
                    const auto &square = mBoard[position.first][y];
                    if (!square) {
                        ret.emplace_back(position.first, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(position.first, y);

                    break;
                }

                // Down
                for (int y = position.second - 1; y >= 0; --y) {
                    const auto &square = mBoard[position.first][y];
                    if (!square) {
                        ret.emplace_back(position.first, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(position.first, y);

                    break;
                }

                // Left
                for (int x = position.first - 1; x >= 0; --x) {
                    const auto &square = mBoard[x][position.second];
                    if (!square) {
                        ret.emplace_back(x, position.second);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, position.second);

                    break;
                }

                // Right
                for (int x = position.first + 1; x < BOARD_SIZE; ++x) {
                    const auto &square = mBoard[x][position.second];
                    if (!square) {
                        ret.emplace_back(x, position.second);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, position.second);

                    break;
                }

                break;

            case Type::Bishop:

                // FIXME: Do this so the same for-loop isn't copy-pasted four times

                // Right and up
                for (int x = position.first + 1, y = position.second + 1;
                     x < BOARD_SIZE && y < BOARD_SIZE; ++x, ++y) {
                    const auto &square = mBoard[x][y];
                    if (!square) {
                        ret.emplace_back(x, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, y);

                    break;
                }

                // Right and down
                for (int x = position.first + 1, y = position.second - 1;
                     x < BOARD_SIZE && y >= 0; ++x, --y) {
                    const auto &square = mBoard[x][y];
                    if (!square) {
                        ret.emplace_back(x, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, y);

                    break;
                }

                // Left and down
                for (int x = position.first - 1, y = position.second - 1;
                     x >= 0 && y >= 0; --x, --y) {
                    const auto &square = mBoard[x][y];
                    if (!square) {
                        ret.emplace_back(x, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, y);

                    break;
                }

                // Left and up
                for (int x = position.first - 1, y = position.second + 1;
                     x >= 0 && y < BOARD_SIZE; --x, ++y) {
                    const auto &square = mBoard[x][y];
                    if (!square) {
                        ret.emplace_back(x, y);
                        continue;
                    }

                    if (square->color != piece->color)
                        ret.emplace_back(x, y);

                    break;
                }

                break;

            case Type::Knight: {
                const array<Position, 8> possibleMoves{
                    {
                        {position.first + 1, position.second + 2},
                        {position.first + 2, position.second + 1},
                        {position.first + 2, position.second - 1},
                        {position.first + 1, position.second - 2},
                        {position.first - 1, position.second - 2},
                        {position.first - 2, position.second - 1},
                        {position.first - 2, position.second + 1},
                        {position.first - 1, position.second + 2},
                    }
                };

                for (const auto &move: possibleMoves) {
                    try {
                        const auto &square = mBoard.at(move.first).at(move.second);

                        if (!square || square->color != piece->color)
                            ret.push_back(move);

                    } catch (out_of_range &) {
                        continue;
                    }
                }

                break;
            }

            case Type::Pawn:
                // TODO: Implement
                break;
        }

        return ret;
    }

    /**
     * Move the piece standing at the 'from' position to the 'to' position.
     * If there is a piece already standing on the 'to' position, that piece
     * is captured, and is therefore removed from the chess board.
     *
     * @param from The position where the moving piece is standing
     * @param to The position to which the piece will move
     */
    void Game::move(const Position &from, const Position &to) {
        // TODO: Implement
    }

    /**
     * Print the current state of the chess board to console.
     * If possibleMoves contain any valid positions, they are highlighted in red.
     *
     * @param possibleMoves Vector containing positions to be highlighted
     */
    void Game::printBoard(const vector <Position> &possibleMoves) const {

        bool evenRow = false;
        for (int y = BOARD_SIZE - 1; y >= 0; --y) {
            bool evenColumn = evenRow;

            wcout << (y + 1);
            for (int x = 0; x < BOARD_SIZE; ++x) {

                // Print arcane ANSI runes so the console output has pretty colors
                // https://stackoverflow.com/a/33206814/18713517
                if (find(possibleMoves.begin(), possibleMoves.end(),
                         Position{x, y}) != possibleMoves.end())
                    wcout << "\033[41m";
                else if (evenColumn)
                    wcout << "\033[44m";

                const auto &square = mBoard[x][y];
                if (!square) {
                    wcout << ' ';

                } else {
                    // Print correct unicode symbol based on the given piece
                    // https://en.wikipedia.org/wiki/Chess_symbols_in_Unicode
                    switch (square->type) {
                        case Type::King:
                            wcout << (square->color == Color::White ? L'\u2654'
                                                                    : L'\u265A');
                            break;
                        case Type::Queen:
                            wcout << (square->color == Color::White ? L'\u2655'
                                                                    : L'\u265B');
                            break;
                        case Type::Rook:
                            wcout << (square->color == Color::White ? L'\u2656'
                                                                    : L'\u265C');
                            break;
                        case Type::Bishop:
                            wcout << (square->color == Color::White ? L'\u2657'
                                                                    : L'\u265D');
                            break;
                        case Type::Knight:
                            wcout << (square->color == Color::White ? L'\u2658'
                                                                    : L'\u265E');
                            break;
                        case Type::Pawn:
                            wcout << (square->color == Color::White ? L'\u2659'
                                                                    : L'\u265F');
                            break;
                    }
                }

                // Reset colors
                wcout << "\033[0m";
                evenColumn = !evenColumn;
            }

            wcout << '\n';
            evenRow = !evenRow;
        }

        wcout << ' ';
        for (int i = 0; i < BOARD_SIZE; ++i)
            wcout << (char) ('a' + i);
        wcout << '\n';
    }
}
