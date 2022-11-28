#pragma once

#include <vector>
#include <optional>

#include "../chess/board.h"
#include "../chess/move.h"

namespace Ai {

    struct Node {
        explicit Node(Chess::Board chessBoard)
                : chessBoard(chessBoard) {
        }

        Node(Chess::Board chessBoard, Node *parent, Chess::Move move)
                : chessBoard(chessBoard),
                  parent(parent),
                  move(move) {
        }

        Chess::Board chessBoard;
        Node *parent{nullptr};
        std::vector<Node> children;

        std::optional<Chess::Move> move;
    };
}
