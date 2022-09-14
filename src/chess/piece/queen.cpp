#include "queen.h"

namespace Chess {
    std::vector<Position> Queen::possibleMoves(const Board &board) const {
        assert(board[mPosition.first][mPosition.second].get() == this);

        // TODO: Implement
        return {};
    }
}
