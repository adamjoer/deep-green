#include "brain.h"

#include <cassert>
#include <limits>
#include <array>
#include <algorithm>

namespace Ai {

    Chess::Move selectMove(const Chess::Board &board) {
        auto chessBoard = Chess::Board(board);
        auto moves = chessBoard.legalMoves();

        int largestNegamaxValue = std::numeric_limits<int>::min();
        int largestNegamaxValueIndex = -1;

        for (int i = 0; i < 6; i += 2) {
            for (int j = 0; j < moves.size(); ++j) {
                Node node(board);
                node.chessBoard.performMove(moves[j]);
                node.move = moves[j];

                auto value = Brain::negaMax(node, i, std::numeric_limits<int>::min(), largestNegamaxValue, -1);
                if (value > largestNegamaxValue) {
                    largestNegamaxValue = value;
                    largestNegamaxValueIndex = j;
                }
            }
        }

        assert(largestNegamaxValueIndex != -1);
        return moves[largestNegamaxValueIndex];
    }

    int Brain::negaMax(Node &node, int depth, int alpha, int beta, int color) {
        // if depth is 0 or node has no children return static evaluation * color.
        if (depth == 0)
            return staticEvaluation(node.chessBoard);

        // Child nodes = generateMoves(node)
        auto moves = node.chessBoard.pseudoLegalMoves();

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
            return staticEvaluation(node.chessBoard);

        // Order child nodes by move priority:
        // Best move from grandparent node - later
        // > capture last piece moved
        // > other captures
        // > center of the board


        // value = INT32_MIN;
        int value = std::numeric_limits<int>::min();

        /*
         * for child in child nodes do
         *      value = max(value, -negamax(child, depth - 1, -beta, -alpha, -color)
         *      alpha = max(alpha, value)
         *      if alpha >= beta then
         *          break;
         */

        for (auto child: node.children) {
            value = std::max(value, -negaMax(child, depth - 1, -beta, -alpha, -color));

            alpha = std::max(alpha, value);
            if (alpha >= beta)
                break;
        }

        // return value
        return value;
    }

    int Brain::staticEvaluation(const Chess::Board &chessBoard) {
        int evaluation = 0;

        // Material count + modifiers
        const Chess::Color color = chessBoard.turnToMove();
        const auto teamOccupiedSquares = chessBoard.teamOccupiedSquares(color);

        const Chess::Color opponent = Chess::oppositeTeam(color);
        const auto opponentOccupiedSquares = chessBoard.teamOccupiedSquares(opponent);

        for (int j = 0; j < 64; ++j) {
            const auto square = Chess::Square(j);

            if (teamOccupiedSquares.isOccupiedAt(square)) {
                auto piece = static_cast<int>(chessBoard.pieceAt(square, color));
                evaluation -= pieceWeights[piece] + positionWeights[static_cast<int>(color)][piece][j];

/*
                std::cout << "White " << Chess::PieceType(piece) << '(' << square << "): pieceWeight="
                          << pieceWeights[piece]
                          << ", positionWeight=" << positionWeights[static_cast<int>(color)][piece][j] << '\n';
*/

            } else if (opponentOccupiedSquares.isOccupiedAt(square)) {
                auto piece = static_cast<int>(chessBoard.pieceAt(square, opponent));
                evaluation += pieceWeights[piece] + positionWeights[static_cast<int>(opponent)][piece][j];

/*
                std::cout << "Black " << Chess::PieceType(piece) << '(' << square << "): pieceWeight="
                          << pieceWeights[piece]
                          << ", positionWeight=" << positionWeights[static_cast<int>(color)][piece][j] << '\n';
*/
            }
        }

        return evaluation;
    }


}
