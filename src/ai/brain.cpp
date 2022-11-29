#include "brain.h"

#include <cassert>
#include <limits>
#include <algorithm>
#include <iostream>

namespace Ai {

    //@formatter:off
    static constexpr int pieceWeights[6]{
        10000,
        900,
        500,
        300,
        300,
        100,
    };

    static constexpr int positionWeights[2][6][64]{
        {
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
            {
                2, 3, 4, 3, 4, 3, 3, 2,
                2, 3, 4, 4, 4, 4, 3, 2,
                3, 4, 4, 4, 4, 4, 4, 3,
                3, 3, 4, 4, 4, 4, 3, 3,
                2, 3, 3, 4, 4, 3, 3, 2,
                2, 2, 2, 3, 3, 2, 2, 2,
                2, 2, 2, 2, 2, 2, 2, 2,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
            {
                9, 9, 11, 10, 11, 9, 9, 9,
                4, 6, 7, 9, 9, 7, 6, 4,
                9, 10, 10, 11, 11, 10, 10, 9,
                8, 8, 8, 9, 9, 8, 8, 8,
                6, 6, 5, 6, 6, 5, 6, 6,
                4, 5,  5,  5,  5,  5,  5,  4,
                3, 4, 4, 6, 6, 4, 4, 3,
                0, 0, 0,  0,  0,  0, 0, 0,
            },
            {
                2, 3, 4, 4, 4, 4, 3, 2,
                4, 7, 7, 7, 7, 7, 7, 4,
                3, 5, 6, 6,  6,  6, 5, 3,
                3, 5, 7, 7, 7, 7, 5, 3,
                4, 5, 6, 8, 8, 6, 5, 4,
                4, 5, 5, -2, -2, 5, 5, 4,
                5, 5, 5, 3, 3, 5, 5, 5,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
            {
                -2, 2,  7,  9,  9,  7,  2,  -2,
                1,  4,  12, 13, 13, 12, 4,  1,
                5,  11, 18, 19, 19, 18, 11, 5,
                3, 10, 14, 14, 14, 14, 10, 3,
                0, 5,  8,  9,  9,  8,  5,  0,
                -3, 1,  3,  4,  4,  3,  1,  -3,
                -5, -3, -1, 0,  0,  -1, -3, -5,
                -7, -5, -4, -2, -2, -4, -5, -7,
            },
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                7,  7,  13, 23, 26, 13, 7,  7,
                -2, -2, 4, 12, 15, 4, -2, -2,
                -3, -3, 2, 9, 11, 2, -3, -3,
                -4, -4, 0, 6, 8,  0, -4, -4,
                -4, -4, 0, 4,  6,  0, -4, -4,
                -1, -1, 1,  5,  6,  1,  -1, -1,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
        },
        {
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                2, 2, 2, 2, 2, 2, 2, 2,
                2, 2, 2, 3, 3, 2, 2, 2,
                2, 3, 3, 4, 4, 3, 3, 2,
                3, 3, 4, 4, 4, 4, 3, 3,
                3, 4, 4, 4, 4, 4, 4, 3,
                2, 3, 4, 4, 4, 4, 3, 2,
                2, 3, 4, 3, 4, 3, 3, 2,
            },
            {
                0, 0, 0,  0,  0,  0, 0, 0,
                3, 4, 4, 6, 6, 4, 4, 3,
                4, 5,  5,  5,  5,  5,  5,  4,
                6, 6, 5, 6, 6, 5, 6, 6,
                8, 8, 8, 9, 9, 8, 8, 8,
                9, 10, 10, 11, 11, 10, 10, 9,
                4, 6, 7, 9, 9, 7, 6, 4,
                9, 9, 11, 10, 11, 9, 9, 9,
            },
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                5, 5, 5, 3, 3, 5, 5, 5,
                4, 5, 5, -2, -2, 5, 5, 4,
                4, 5, 6, 8, 8, 6, 5, 4,
                3, 5, 7, 7, 7, 7, 5, 3,
                3, 5, 6, 6,  6,  6, 5, 3,
                4, 7, 7, 7, 7, 7, 7, 4,
                2, 3, 4, 4, 4, 4, 3, 2,
            },
            {
                -7, -5, -4, -2, -2, -4, -5, -7,
                -5, -3, -1, 0,  0,  -1, -3, -5,
                -3, 1,  3,  4,  4,  3,  1,  -3,
                0, 5,  8,  9,  9,  8,  5,  0,
                3, 10, 14, 14, 14, 14, 10, 3,
                5,  11, 18, 19, 19, 18, 11, 5,
                1,  4,  12, 13, 13, 12, 4,  1,
                -2, 2,  7,  9,  9,  7,  2,  -2,
            },
            {
                0, 0, 0, 0, 0, 0, 0, 0,
                -1, -1, 1,  5,  6,  1,  -1, -1,
                -4, -4, 0, 4,  6,  0, -4, -4,
                -4, -4, 0, 6, 8,  0, -4, -4,
                -3, -3, 2, 9, 11, 2, -3, -3,
                -2, -2, 4, 12, 15, 4, -2, -2,
                7,  7,  13, 23, 26, 13, 7,  7,
                0, 0, 0, 0, 0, 0, 0, 0,
            },
        },
    };
    //@formatter:on

    int negaMax(Node &node, int depth, int alpha, int beta, int color = 1);

    int staticEvaluation(const Chess::Board &chessBoard);

    void selectMove(QPromise<Chess::Move> &promise, const Chess::Board &board) {
        auto moves = board.pseudoLegalMoves();

        int largestNegamaxValue = std::numeric_limits<int>::min();
        int largestNegamaxValueIndex = -1;

        for (int i = 0; i < 6; i += 2) {
            for (int j = 0; j < moves.size(); ++j) {
                promise.suspendIfRequested();
                if (promise.isCanceled())
                    return;

                Node node(board);
                node.chessBoard.performMove(moves[j]);
                node.move = moves[j];

                auto value = negaMax(node, i, std::numeric_limits<int>::min(), largestNegamaxValue, -1);
                if (value > largestNegamaxValue) {
                    largestNegamaxValue = value;
                    largestNegamaxValueIndex = j;
                }
            }
        }

        assert(largestNegamaxValueIndex != -1);
        promise.addResult(moves[largestNegamaxValueIndex]);
    }

    int negaMax(Node &node, int depth, int alpha, int beta, int color) {
        // if depth is 0 or node has no children return static evaluation * color.
        if (depth <= 0)
            return color * staticEvaluation(node.chessBoard);

        // Child nodes = generateMoves(node)
        auto moves = node.chessBoard.pseudoLegalMoves();

        // Order child nodes by move priority:
        // Best move from grandparent node - later
        // > capture last piece moved
        // > other captures
        // > center of the board
        if (node.move) {
            std::sort(moves.begin(), moves.end(), [&](Chess::Move a, Chess::Move b) {
                if (a.to == node.move->to) {
                    if (b.to != node.move->to)
                        return true;
                    // else compare in a different way

                }
                return false;
            });
        }

        for (auto move: moves) {
            // Copy board
            // Perform move
            // Create node
            // Add to children vector
            Chess::Board chessBoard(node.chessBoard); // TODO: Needs constructor without copying move vector
            chessBoard.performMove(move);
            node.children.emplace_back(chessBoard, &node, move);
        }

        if (node.children.empty())
            return color * staticEvaluation(node.chessBoard);

        int value = std::numeric_limits<int>::min();

        for (auto child: node.children) {
            value = std::max(value, -negaMax(child, depth - 1, -beta, -alpha, -color));

            alpha = std::max(alpha, value);
            if (alpha >= beta)
                break;
        }

        // return value
        return value;
    }

    int staticEvaluation(const Chess::Board &chessBoard) {
        int evaluation = 0;

        // Material count + modifiers
        const auto whiteOccupiedSquares = chessBoard.teamOccupiedSquares(Chess::Color::White);

        const auto blackOccupiedSquares = chessBoard.teamOccupiedSquares(Chess::Color::Black);

        for (int j = 0; j < 64; ++j) {
            const auto square = Chess::Square(j);

            if (whiteOccupiedSquares.isOccupiedAt(square)) {
                auto piece = static_cast<int>(chessBoard.pieceAt(square, Chess::Color::White));
                evaluation += (pieceWeights[piece] + positionWeights[0][piece][j]);

            } else if (blackOccupiedSquares.isOccupiedAt(square)) {
                auto piece = static_cast<int>(chessBoard.pieceAt(square, Chess::Color::Black));
                evaluation -= (pieceWeights[piece] + positionWeights[1][piece][j]);
            }
        }

        return evaluation;
    }
}
