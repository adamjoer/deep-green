#include "brain.h"

#include <limits>
#include <array>
#include <algorithm>

namespace Ai {
    int Brain::negaMax(Node &node, int depth, int alpha, int beta, int color) {
        // if depth is 0 or node has no children return static evaluation * color.
        if (depth == 0)
            return staticEvaluation(node.chessBoard);

        // Child nodes = generateMoves(node)
        auto moves = node.chessBoard.pseudoLegalMoves();
        for (auto move: moves) {
            // Copy board
            // Perform move
            // Create node
            // Add to children vector

            Chess::Board chessBoard(node.chessBoard);
            chessBoard.performMove(move);
            node.children.emplace_back(chessBoard, &node, move);
        }

        if (node.children.empty())
            return staticEvaluation(node.chessBoard);

        // Order child nodes by move priority: Best move from parent node > capture last piece moved > other captures > center of the board


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
                evaluation += pieceWeights[piece] + positionWeights[static_cast<int>(color)][piece][j];

/*
                std::cout << "White " << Chess::PieceType(piece) << '(' << square << "): pieceWeight="
                          << pieceWeights[piece]
                          << ", positionWeight=" << positionWeights[static_cast<int>(color)][piece][j] << '\n';
*/

            } else if (opponentOccupiedSquares.isOccupiedAt(square)) {
                auto piece = static_cast<int>(chessBoard.pieceAt(square, opponent));
                evaluation -= pieceWeights[piece] + positionWeights[static_cast<int>(opponent)][piece][j];

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
